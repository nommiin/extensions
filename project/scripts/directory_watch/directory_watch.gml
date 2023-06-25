function __directory_watch(index, callback, interval) constructor {
	self.__index = index;
	self.__callback = callback;
	self.__source = time_source_create(time_source_global, interval / 1000, time_source_units_seconds, method(self, self.__update), [], -1);
	time_source_start(self.__source);
	
	static close = function() {
		time_source_stop(self.__source);
		return directory_watch_destroy(self.__index);
	}
	
	static __update = function() {
		if (directory_watch_update(self.__index)) {
			self.__callback();
		}
	}
}

function directory_watch(path, filter, callback, subtree=true, interval=1000) {
	if (!directory_exists(path)) {
		show_error("Could not call directory_watch, non-existent path was provided as first argument", false);	
	}
	return new __directory_watch(directory_watch_create(path, subtree, filter), callback, interval);
}

#macro FILE_NOTIFY_CHANGE_FILE_NAME    0x00000001   
#macro FILE_NOTIFY_CHANGE_DIR_NAME     0x00000002   
#macro FILE_NOTIFY_CHANGE_ATTRIBUTES   0x00000004   
#macro FILE_NOTIFY_CHANGE_SIZE         0x00000008   
#macro FILE_NOTIFY_CHANGE_LAST_WRITE   0x00000010   
#macro FILE_NOTIFY_CHANGE_LAST_ACCESS  0x00000020   
#macro FILE_NOTIFY_CHANGE_CREATION     0x00000040   
#macro FILE_NOTIFY_CHANGE_SECURITY     0x00000100   
#macro FILE_NOTIFY_ALL                 FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_LAST_ACCESS | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SECURITY