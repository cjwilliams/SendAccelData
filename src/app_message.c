#include <pebble.h>

Window *window;	

TextLayer *text_layer;
char *text_buffer;	

// Key values for AppMessage Dictionary
enum {
  ACCEL_X_KEY,	
  ACCEL_Y_KEY,
  ACCEL_Z_KEY
};

void print_accel_data(int16_t accel_x, int16_t accel_y, int16_t accel_z) {
  snprintf(text_buffer, 100, "x:%i, y:%i, z:%i", accel_x, accel_y, accel_z);
  text_layer_set_text(text_layer, text_buffer);
}

// Write message to buffer & send
void send_message(int16_t accel_x, int16_t accel_y, int16_t accel_z){
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
}

// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {	
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}

void accel_data_handler(AccelData *data, uint32_t num_samples) {
  send_message(data->x, data->y, data->z);
  print_accel_data(data->x, data->y, data->z);
}

void window_load(Window *window) {
  text_buffer = malloc(100);	
  text_layer = text_layer_create(GRect(0,0,144,154));
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
}

void window_unload(Window *window) {
  text_layer_destroy(text_layer);
  free(text_buffer);
}

void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
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
