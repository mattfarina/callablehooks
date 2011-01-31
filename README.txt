Callable Hooks enables any PHP callable item to be an implementation of a hook.

This module is currently a proof of concept.

This works for new hooks defined in module and does not work for Drupal 7 core
hooks or hooks defined by modules that don't depend on Callable Hooks.

Defining Hooks:
When a module wants to implement a hook it needs to register it. For example:
callablehooks()->register_hook('example_foo', 'callablehooks_example_foo_callback')
  ->register_hook('example_bar', array('CallableHooksExample', 'bar'));

Notice registering hooks is chainable. The first use of register_hook registers
the function callablehooks_example_foo_callback as an implementation of the hook
example_foo. The second use registers the method bar on the class
CallableHooksExample as an implementation of example_bar.

Implementing Hooks:
There are two useful ways to call hooks. The first is similar to
the core function of module_invoke_all:

$results = callablehooks()->invokeAll('example_foo', $arg1, $arg2, $arg3);

The second method is similar to how module_invoke is often used when iterating
over a set of hooks.

$callbacks = callablehooks()->invoke_iterator('example_bar');

foreach ($callbacks as $callback) {
  // Implement a single hook with arguments.
  $baz = $callback->invoke($arg1, $arg2);
}