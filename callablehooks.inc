<?php

/**
 * @file
 * The classes that make callable hooks happen.
 */

/**
 * The controller clas for Callable Hooks.
 */
class CallableHooks {
  
  /**
   * @var The hooks with their assoriated callbacks.
   *
   * @todo Should this be cached? (measurements needed with and without opcode cache.)
   */
  public $hooks = array();

  /**
   * Register a hook to be called in the future.
   *
   * @param string $hook
   *   The name of the hook to register to.
   * @param mixed $callback
   *   Anything callable. See http://us2.php.net/manual/en/language.pseudo-types.php#language.types.callback
   *
   * return CallableHooks
   *   We return the current object so we can chain.
   */
  function register_hook($hook, $callback) {
    if (!isset($this->hooks[$hook])) {
      $this->hooks[$hook] = array();
    }

    $this->hooks[$hook][] = $callback;

    return $this;
  }

  /**
   * Call a hook.
   *
   * @param string $hook
   *   The name of the hook to be called.
   * @param mixed ...
   *   All other arguments are passed onto the callbacks.
   *
   * @return array
   *   An array of the results from the callback.
   */
  function invokeAll() {
    $args = func_get_args();
    $hook = $args[0];
    unset($args[0]);
    $return = array();
    foreach ($this->hooks[$hook] as $callback) {
      $result = call_user_func_array($callback, $args);
      if (isset($result) && is_array($result)) {
        $return = array_merge_recursive($return, $result);
      }
      elseif (isset($result)) {
        $return[] = $result;
      }
    }

    return $return;
  }

  /**
   * Get a hook iterator. This allows you to iterate over each hook implementation
   * act on the results.
   *
   * @param string $hook
   *   The name of the hook to be called.
   *
   * @return CallableHooksIterator
   *   An object that can be iterated over to call each implementation of a hook.
   *
   */
  function invoke_iterator() {
    $args = func_get_args();
    $hook = $args[0];
    unset($args[0]);

    return new CallableHooksIterator($this->hooks[$hook] ? $this->hooks[$hook] : array());
  }
}

/**
 * Allow you to iterate over the implementations for a hook.
 */
class CallableHooksIterator implements Countable, Iterator {

  public $callbacks = array();

  public $position = 0;

  function __construct($callbacks) {
    $this->callbacks = $callbacks;
  }

  /**
   * Implementation of count for the Countable interface.
   */
  function count() {
    return count($callbacks);
  }
  
  function current() {
    return new CallableHooksIteratorItem($this->callbacks[$this->position]);
  }
  
  function key() {
    return $this->position;
  }
  
  function next() {
    ++$this->position;
  }
  
  function rewind() {
    $this->position = 0;
  }
  
  function valid() {
    return isset($this->callbacks[$this->position]);
  }
}

class CallableHooksIteratorItem {
  
  public $callback = null;
  
  function __construct($callback) {
    $this->callback = $callback;
  }
  
  function invoke() {
    $args = func_get_args();
    return call_user_func_array($this->callback, $args);
  }
}