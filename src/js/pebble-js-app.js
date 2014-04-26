// Called when JS is ready
Pebble.addEventListener("ready",
							function(e) {
							});
												
// Called when incoming message from the Pebble is received
Pebble.addEventListener("appmessage",
							function(e) {
								console.log("X: " + e.payload.accel_x_data + ", Y: " + e.payload.accel_y_data + ", Z: " + e.payload.accel_z_data);
							});