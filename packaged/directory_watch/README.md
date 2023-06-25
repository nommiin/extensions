# directory_watch
A basic Windows-only extension to allow for callbacks whenever a change notification is recieved, support for this extension is limited and functionality can change at any time. Basic usage is outlined below!

# Usage
1. Import directory_watch.yymps
2. Call `directory_watch` and provide a `path` (string), `filter` (integer, see `FILE_NOTIFY_*` macros), `callback` (function), and optional `subtree` (bool) & `interval` (integer) arguments
3. From here, the watch will automatically be updated in a time source. Use the `.close` method on the returned object to end the time source and stop checking for updates

# Example
```
// watches the "saves" directory (and nested folders) for all changes every 500ms
self.watcher = directory_watch("saves\\", FILE_NOTIFY_ALL, function() {
  show_debug_message("the saves directory has been updated!");
  // do some stuff
}, true, 500);
```
