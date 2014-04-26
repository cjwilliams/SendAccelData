#include <pebble.h>

Window *window;	
	
// Key values for AppMessage Dictionary
enum {
	ACCEL_X_KEY,	
	ACCEL_Y_KEY,
	ACCEL_Z_KEY
};

// Write message to buffer & send
void send_message(uint8_t accel_x, uint8_t accel_y, uint8_t accel_z){
	DictionaryIterator *iter;
	
	app_message_outbox_begin(&iter);
	dict_write_uint8(iter, ACCEL_X_KEY, accel_x);
	dict_write_uint8(iter, ACCEL_Y_KEY, accel_y);
	dict_write_uint8(iter, ACCEL_Z_KEY, accel_z);
	
	dict_write_end(iter);
  app_message_outbox_send();
}

// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {
//	Tuple *tuple;
	
//	tuple = dict_find(received, STATUS_KEY);
//	if(tuple) {
//		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Status: %d", (int)tuple->value->uint32); 
//	}
	
//	tuple = dict_find(received, MESSAGE_KEY);
//	if(tuple) {
//		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", tuple->value->cstring);
//	}
}

// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {	
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}

void accel_data_handler(AccelData *data, uint32_t num_samples) {
//	for(int i=0; i < num_samples; i++) {
//		AccelData *sample = data[i];
		send_message(data->x, data->y, data->z);
//	}
}

void init(void) {
	window = window_create();
	window_stack_push(window, true);
	
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);
		
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
	accel_data_service_subscribe(1, accel_data_handler);
}

void deinit(void) {
	app_message_deregister_callbacks();
	window_destroy(window);
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}