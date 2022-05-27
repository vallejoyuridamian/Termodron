

void sendToOnBoard(uint8_t count_data, const uint8_t* data){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_serial_control_pack(system_id, component_id, &msg, SERIAL_CONTROL_DEV_TELEM1, SERIAL_CONTROL_FLAG_REPLY, 0, 0, count_data, data);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

