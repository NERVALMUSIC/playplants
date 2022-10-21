import 'dart:async';
import 'dart:io';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_midi_command/flutter_midi_command.dart';
import 'package:flutter_midi_command/flutter_midi_command_messages.dart';

class ControllerPage extends StatelessWidget {
  final MidiDevice device;
  const ControllerPage(this.device);
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Plant Control'),
      ),
      body: MidiControls(device),
    );
  }
}

class MidiControls extends StatefulWidget {
  final MidiDevice device;

  const MidiControls(this.device);

  @override
  MidiControlsState createState() {
    return MidiControlsState();
  }
}

class MidiControlsState extends State<MidiControls> {
  // StreamSubscription<String> _setupSubscription;
  StreamSubscription<MidiPacket>? _rxSubscription;
  final MidiCommand _midiCommand = MidiCommand();
  final _channel = TextEditingController();
  final _e1 = TextEditingController();
  final _e2 = TextEditingController();
  final _e3 = TextEditingController();
  final _e4 = TextEditingController();
  final _e5 = TextEditingController();
  final _e6 = TextEditingController();
  final _e7 = TextEditingController();
  final _e8 = TextEditingController();
  final _e9 = TextEditingController();
  final _e10 = TextEditingController();
  final _e11 = TextEditingController();
  final _e12 = TextEditingController();
  double _sensitivity = 25;

  @override
  void initState() {
    _rxSubscription = _midiCommand.onMidiDataReceived?.listen((packet) {
      var data = packet.data;
      var status = data[0];
      if (status == 0xF8) {
        // Beat
        return;
      }
      if (status == 0xFE) {
        // Active sense;
        return;
      }
      if (data.length >= 2) {
        var rawStatus = status & 0xF0; // without channel
        //var channel = (status & 0x0F);
        //var d1 = data[1];
        switch (rawStatus) {
          case 0xB0: // CC
            break;
        }
      }
    });

    super.initState();
  }

  @override
  void dispose() {
    // _setupSubscription?.cancel();
    _rxSubscription?.cancel();
    _channel.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: ListView(
        children: <Widget>[
          ListTile(
            leading: const Icon(Icons.hexagon_outlined),
            title: const Text('Channel'),
            subtitle: TextField(
                controller: _channel,
                keyboardType: TextInputType.number,
                inputFormatters: <TextInputFormatter>[
                  FilteringTextInputFormatter.digitsOnly
                ]),
          ),
          const ListTile(
            leading: Icon(Icons.touch_app_outlined),
            title: Text('Sensitivity'),
          ),
          Slider(
            value: _sensitivity,
            max: 127,
            min: 0,
            label: _sensitivity.round().toString(),
            onChanged: (double value) {
              setState(() {
                _sensitivity = value;
              });
            },
          ),
          GridView.count(
            crossAxisCount: 3,
            physics: const NeverScrollableScrollPhysics(),
            shrinkWrap: true, // You won't see infinite size error
            children: <Widget>[
              Flexible(
                fit: FlexFit.tight,
                child: ListTile(
                  leading: const Icon(Icons.eco_outlined),
                  title: const Text('1'),
                  subtitle: TextField(
                      controller: _e1,
                      keyboardType: TextInputType.number,
                      inputFormatters: <TextInputFormatter>[
                        FilteringTextInputFormatter.digitsOnly
                      ]),
                ),
              ),
              Flexible(
                fit: FlexFit.tight,
                child: ListTile(
                  leading: const Icon(Icons.eco_outlined),
                  title: const Text('2'),
                  subtitle: TextField(
                      controller: _e2,
                      keyboardType: TextInputType.number,
                      inputFormatters: <TextInputFormatter>[
                        FilteringTextInputFormatter.digitsOnly
                      ]),
                ),
              ),
              Flexible(
                fit: FlexFit.tight,
                child: ListTile(
                  leading: const Icon(Icons.eco_outlined),
                  title: const Text('3'),
                  subtitle: TextField(
                      controller: _e3,
                      keyboardType: TextInputType.number,
                      inputFormatters: <TextInputFormatter>[
                        FilteringTextInputFormatter.digitsOnly
                      ]),
                ),
              ),
              Flexible(
                fit: FlexFit.tight,
                child: ListTile(
                  leading: const Icon(Icons.eco_outlined),
                  title: const Text('4'),
                  subtitle: TextField(
                      controller: _e4,
                      keyboardType: TextInputType.number,
                      inputFormatters: <TextInputFormatter>[
                        FilteringTextInputFormatter.digitsOnly
                      ]),
                ),
              ),
              Flexible(
                fit: FlexFit.tight,
                child: ListTile(
                  leading: const Icon(Icons.eco_outlined),
                  title: const Text('5'),
                  subtitle: TextField(
                      controller: _e5,
                      keyboardType: TextInputType.number,
                      inputFormatters: <TextInputFormatter>[
                        FilteringTextInputFormatter.digitsOnly
                      ]),
                ),
              ),
              Flexible(
                fit: FlexFit.tight,
                child: ListTile(
                  leading: const Icon(Icons.eco_outlined),
                  title: const Text('6'),
                  subtitle: TextField(
                      controller: _e6,
                      keyboardType: TextInputType.number,
                      inputFormatters: <TextInputFormatter>[
                        FilteringTextInputFormatter.digitsOnly
                      ]),
                ),
              ),
              Flexible(
                fit: FlexFit.tight,
                child: ListTile(
                  leading: const Icon(Icons.eco_outlined),
                  title: const Text('7'),
                  subtitle: TextField(
                      controller: _e7,
                      keyboardType: TextInputType.number,
                      inputFormatters: <TextInputFormatter>[
                        FilteringTextInputFormatter.digitsOnly
                      ]),
                ),
              ),
              Flexible(
                fit: FlexFit.tight,
                child: ListTile(
                  leading: const Icon(Icons.eco_outlined),
                  title: const Text('8'),
                  subtitle: TextField(
                      controller: _e8,
                      keyboardType: TextInputType.number,
                      inputFormatters: <TextInputFormatter>[
                        FilteringTextInputFormatter.digitsOnly
                      ]),
                ),
              ),
              Flexible(
                fit: FlexFit.tight,
                child: ListTile(
                  leading: const Icon(Icons.eco_outlined),
                  title: const Text('9'),
                  subtitle: TextField(
                      controller: _e9,
                      keyboardType: TextInputType.number,
                      inputFormatters: <TextInputFormatter>[
                        FilteringTextInputFormatter.digitsOnly
                      ]),
                ),
              ),
              Flexible(
                fit: FlexFit.tight,
                child: ListTile(
                  leading: const Icon(Icons.eco_outlined),
                  title: const Text('10'),
                  subtitle: TextField(
                      controller: _e10,
                      keyboardType: TextInputType.number,
                      inputFormatters: <TextInputFormatter>[
                        FilteringTextInputFormatter.digitsOnly
                      ]),
                ),
              ),
              Flexible(
                fit: FlexFit.tight,
                child: ListTile(
                  leading: const Icon(Icons.eco_outlined),
                  title: const Text('11'),
                  subtitle: TextField(
                      controller: _e11,
                      keyboardType: TextInputType.number,
                      inputFormatters: <TextInputFormatter>[
                        FilteringTextInputFormatter.digitsOnly
                      ]),
                ),
              ),
              Flexible(
                fit: FlexFit.tight,
                child: ListTile(
                  leading: const Icon(Icons.eco_outlined),
                  title: const Text('12'),
                  subtitle: TextField(
                      controller: _e12,
                      keyboardType: TextInputType.number,
                      inputFormatters: <TextInputFormatter>[
                        FilteringTextInputFormatter.digitsOnly
                      ]),
                ),
              ),
            ],
          ),
        ],
      ),
      floatingActionButton: FloatingActionButton(
        // When the user presses the button, show an alert dialog containing
        // the text that the user has entered into the text field.
        onPressed: () {
          if (_channel.text.isNotEmpty) {
            CCMessage(
              channel: int.parse(_channel.text),
              controller: 102,
              value: int.parse(_channel.text),
            ).send();
            sleep(const Duration(milliseconds: 100));
            CCMessage(
              channel: int.parse(_channel.text),
              controller: 103,
              value: _sensitivity.round().toInt(),
            ).send();
            sleep(const Duration(milliseconds: 100));
          }
          if (_e1.text.isNotEmpty) {
            NoteOffMessage(
              channel: 1,
              note: int.parse(_e1.text),
              velocity: 0,
            ).send();
          }
          if (_e2.text.isNotEmpty) {
            NoteOffMessage(
              channel: 2,
              note: int.parse(_e2.text),
              velocity: 0,
            ).send();
          }
          if (_e3.text.isNotEmpty) {
            NoteOffMessage(
              channel: 3,
              note: int.parse(_e3.text),
              velocity: 0,
            ).send();
          }
          if (_e4.text.isNotEmpty) {
            NoteOffMessage(
              channel: 4,
              note: int.parse(_e4.text),
              velocity: 0,
            ).send();
          }
          if (_e5.text.isNotEmpty) {
            NoteOffMessage(
              channel: 5,
              note: int.parse(_e5.text),
              velocity: 0,
            ).send();
          }
          if (_e6.text.isNotEmpty) {
            NoteOffMessage(
              channel: 6,
              note: int.parse(_e6.text),
              velocity: 0,
            ).send();
          }
          if (_e7.text.isNotEmpty) {
            NoteOffMessage(
              channel: 7,
              note: int.parse(_e7.text),
              velocity: 0,
            ).send();
          }
          if (_e8.text.isNotEmpty) {
            NoteOffMessage(
              channel: 8,
              note: int.parse(_e8.text),
              velocity: 0,
            ).send();
          }
          if (_e9.text.isNotEmpty) {
            NoteOffMessage(
              channel: 9,
              note: int.parse(_e9.text),
              velocity: 0,
            ).send();
          }
          if (_e10.text.isNotEmpty) {
            NoteOffMessage(
              channel: 10,
              note: int.parse(_e10.text),
              velocity: 0,
            ).send();
          }
          if (_e11.text.isNotEmpty) {
            NoteOffMessage(
              channel: 11,
              note: int.parse(_e11.text),
              velocity: 0,
            ).send();
          }
          if (_e12.text.isNotEmpty) {
            NoteOffMessage(
              channel: 12,
              note: int.parse(_e12.text),
              velocity: 0,
            ).send();
          }
          showDialog(
            context: context,
            builder: (context) {
              return const AlertDialog(
                content: Text("Data sent!"),
              );
            },
          );
        },
        tooltip: 'Send configuration',
        child: const Icon(Icons.energy_savings_leaf_outlined),
      ),
    );
  }
}
