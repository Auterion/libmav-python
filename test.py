import unittest
import sys
sys.path.append('./cmake-build-debug')

import libmav


BIG_MESSAGE = '''
<mavlink>
    <enums>
        <enum name="SOME_ENUM">
            <entry value="123" name="SOME_ENUM_A"/>
            <entry value="124" name="SOME_ENUM_B"/>
        </enum>
    </enums>
    <messages>
        <message id="0" name="HEARTBEAT">
            <field type="uint8_t" name="type">Type of the MAV (quadrotor, helicopter, etc., up to 15 types, defined in MAV_TYPE ENUM)</field>
            <field type="uint8_t" name="autopilot">Autopilot type / class. defined in MAV_AUTOPILOT ENUM</field>
            <field type="uint8_t" name="base_mode">System mode bitfield, see MAV_MODE_FLAGS ENUM in mavlink/include/mavlink_types.h</field>
            <field type="uint32_t" name="custom_mode">A bitfield for use for autopilot-specific flags.</field>
            <field type="uint8_t" name="system_status">System status flag, see MAV_STATE ENUM</field>
            <field type="uint8_t" name="mavlink_version">MAVLink version, not writable by user, gets added by protocol because of magic data type: uint8_t_mavlink_version</field>
        </message>
        <message id="9915" name="BIG_MESSAGE">
            <field type="uint8_t" name="uint8_field">description</field>
            <field type="int8_t" name="int8_field">description</field>
            <field type="uint16_t" name="uint16_field">description</field>
            <field type="int16_t" name="int16_field">description</field>
            <field type="uint32_t" name="uint32_field">description</field>
            <field type="int32_t" name="int32_field">description</field>
            <field type="uint64_t" name="uint64_field">description</field>
            <field type="int64_t" name="int64_field">description</field>
            <field type="double" name="double_field">description</field>
            <field type="float" name="float_field">description</field>
            <field type="char[20]" name="char_arr_field">description</field>
            <field type="float[3]" name="float_arr_field">description</field>
            <field type="int32_t[3]" name="int32_arr_field">description</field>
        </message>
    </messages>
</mavlink>
'''


class TestMessageSet(unittest.TestCase):
    def testMessageSet(self):
        message_set = libmav.MessageSet()
        message_set.add_from_xml_string(BIG_MESSAGE)
        self.assertEqual(len(message_set), 2)
        self.assertTrue('BIG_MESSAGE' in message_set)
        self.assertTrue(9915 in message_set)
        self.assertFalse(12 in message_set)
        self.assertFalse('OTHER_MESSAGE' in message_set)
        self.assertEqual(message_set.id_for_message('BIG_MESSAGE'), 9915)
        self.assertEqual(message_set.enum('SOME_ENUM_A'), 123)
        self.assertEqual(message_set.enum('SOME_ENUM_B'), 124)

    def testCreateMessage(self):
        message_set = libmav.MessageSet()
        message_set.add_from_xml_string(BIG_MESSAGE)

        message = message_set.create('BIG_MESSAGE')
        self.assertEqual(message.name, 'BIG_MESSAGE')
        self.assertEqual(message.type.name, 'BIG_MESSAGE')
        self.assertEqual(message.id, 9915)


class TestMessage(unittest.TestCase):
    def setUp(self) -> None:
        self.message_set = libmav.MessageSet()
        self.message_set.add_from_xml_string(BIG_MESSAGE)

    def testMessageSetGet(self):
        message = self.message_set.create('BIG_MESSAGE')
        message['uint8_field'] = 1
        message['int8_field'] = 2
        message['uint16_field'] = 3
        message['int16_field'] = 4
        message['uint32_field'] = 5
        message['int32_field'] = 6
        message['uint64_field'] = 7
        message['int64_field'] = 8
        message['double_field'] = 9.0
        message['float_field'] = 10.0
        message['char_arr_field'] = 'Hello world'
        message['float_arr_field'] = [1.0, 2.0, 3.0]
        message['int32_arr_field'] = [4, 5, 6]

        self.assertEqual(message['uint8_field'], 1)
        self.assertEqual(message['int8_field'], 2)
        self.assertEqual(message['uint16_field'], 3)
        self.assertEqual(message['int16_field'], 4)
        self.assertEqual(message['uint32_field'], 5)
        self.assertEqual(message['int32_field'], 6)
        self.assertEqual(message['uint64_field'], 7)
        self.assertEqual(message['int64_field'], 8)
        self.assertEqual(message['double_field'], 9.0)
        self.assertEqual(message['float_field'], 10.0)
        self.assertEqual(message['char_arr_field'], 'Hello world')
        self.assertEqual(message['float_arr_field'], [1.0, 2.0, 3.0])
        self.assertEqual(message['int32_arr_field'], [4, 5, 6])

    def testMessageFloatPackUnpack(self):
        message = self.message_set.create('BIG_MESSAGE')
        message.set_as_float_pack('float_field', 12)
        self.assertEqual(message.get_as_float_unpack('float_field'), 12)

    def testSetFromDict(self):
        message = self.message_set.create('BIG_MESSAGE')

        original = {
            '_id': 9915,
            '_name': 'BIG_MESSAGE',
            'uint8_field': 1,
            'int8_field': 2,
            'uint16_field': 3,
            'int16_field': 4,
            'uint32_field': 5,
            'int32_field': 6,
            'uint64_field': 7,
            'int64_field': 8,
            'double_field': 9.4,
            'float_field': 10.5,
            'char_arr_field': 'Hello world',
            'float_arr_field': [1.0, 2.0, 3.0],
            'int32_arr_field': [4, 5, 6]
        }

        message.set_from_dict(original)
        output = message.to_dict()
        self.assertEqual(original, output)

class TestPhysical(unittest.TestCase):
    def setUp(self) -> None:
        self.message_set = libmav.MessageSet()
        self.message_set.add_from_xml_string(BIG_MESSAGE)

        self.big_message = self.message_set.create('BIG_MESSAGE').set_from_dict({
            'uint8_field': 1,
            'int8_field': 2,
            'uint16_field': 3,
            'int16_field': 4,
            'uint32_field': 5,
            'int32_field': 6,
            'uint64_field': 7,
            'int64_field': 8,
            'double_field': 9.4,
            'float_field': 10.5,
            'char_arr_field': 'Hello world',
            'float_arr_field': [1.0, 2.0, 3.0],
            'int32_arr_field': [4, 5, 6]
        })

    def testTCPConnection(self):

        heartbeat = self.message_set.create('HEARTBEAT').set_from_dict({
            'type': 1,
            'autopilot': 2,
            'base_mode': 3,
            'custom_mode': 4,
            'system_status': 5,
            'mavlink_version': 6
        })
        server_physical = libmav.TCPServer(193413)
        server_runtime = libmav.NetworkRuntime(self.message_set, heartbeat, server_physical)

        client_physical = libmav.TCPClient('127.0.0.1', 193413)
        client_runtime = libmav.NetworkRuntime(self.message_set, heartbeat, client_physical)

        server_conn = server_runtime.await_connection(2000)
        client_conn = client_runtime.await_connection(2000)

        expectation = client_conn.expect('BIG_MESSAGE')
        server_conn.send(self.big_message)
        response = client_conn.receive(expectation, 100)

        self.assertEqual(self.big_message.to_dict(), response.to_dict())

        expectation = server_conn.expect('BIG_MESSAGE')
        client_conn.send(self.big_message)
        response = server_conn.receive(expectation, 100)
        self.assertEqual(self.big_message.to_dict(), response.to_dict())

    def testUDPConnection(self):
        heartbeat = self.message_set.create('HEARTBEAT').set_from_dict({
            'type': 1,
            'autopilot': 2,
            'base_mode': 3,
            'custom_mode': 4,
            'system_status': 5,
            'mavlink_version': 6
        })
        server_physical = libmav.UDPServer(193413)
        server_runtime = libmav.NetworkRuntime(self.message_set, heartbeat, server_physical)

        client_physical = libmav.UDPClient('127.0.0.1', 193413)
        client_runtime = libmav.NetworkRuntime(self.message_set, heartbeat, client_physical)

        server_conn = server_runtime.await_connection(2000)
        client_conn = client_runtime.await_connection(2000)

        expectation = client_conn.expect('BIG_MESSAGE')
        server_conn.send(self.big_message)
        response = client_conn.receive(expectation, 100)

        self.assertEqual(self.big_message.to_dict(), response.to_dict())

        expectation = server_conn.expect('BIG_MESSAGE')
        client_conn.send(self.big_message)
        response = server_conn.receive(expectation, 100)
        self.assertEqual(self.big_message.to_dict(), response.to_dict())


if __name__ == '__main__':
    unittest.main()
