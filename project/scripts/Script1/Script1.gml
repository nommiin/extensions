function create() {
	dir = directory_watch("test\\", FILE_NOTIFY_ALL, function() {
		show_debug_message("changed file");
	}, true, 1000);
	
	show_debug_message(dir);
}

function update() {
		
}