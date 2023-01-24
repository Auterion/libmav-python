import libmav

c = libmav.MessageSet('/home/thomas/projects/mavlink/message_definitions/v1.0/common.xml')
message = c.createMessage("CHANGE_OPERATOR_CONTROL")

print(message['passkey'])

message['passkey'] = "Hello world"

print(message['passkey'])

print('All done!')
