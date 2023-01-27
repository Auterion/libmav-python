import sys
# the mock-0.3.1 dir contains testcase.py, testutils.py & mock.py
sys.path.append('./cmake-build-debug')

import libmav

message_set = libmav.MessageSet('/home/thomas/projects/mavlink/message_definitions/v1.0/common.xml')

message_set.addFromXMLString('''
<mavlink>
    <messages>
        <message id="9912" name="TEMPERATURE_MEASUREMENT">
            <field type="float" name="temperature">The measured temperature in degress C</field>
        </message>
    </messages>
</mavlink>
''')

temp_message = message_set.create("TEMPERATURE_MEASUREMENT")
temp_message["temperature"] = 12.6

message = message_set.create("CHANGE_OPERATOR_CONTROL")

string = message['passkey']
print(len(string))
print(string)

message['passkey'] = "Hello world"

print(message['passkey'])

for key in message.type.keys():
    print('KEY ' + key)

if 'target_system' in message:
    message['target_system'] = 1
else:
    message['target_system'] = 2

for key, value in message:
    print('-> ' + key + ": " + str(value))


print('HEARTBEAT' in message_set)
print(0 in message_set)

print('All done!')

