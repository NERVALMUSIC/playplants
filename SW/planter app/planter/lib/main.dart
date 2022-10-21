import 'dart:async';

import 'package:flutter/material.dart';
import 'package:flutter_midi_command/flutter_midi_command.dart';
import 'controller.dart';

void main() => runApp(MyApp());

class MyApp extends StatefulWidget {
  @override
  MyAppState createState() => MyAppState();
}

class MyAppState extends State<MyApp> {
  StreamSubscription<String>? _setupSubscription;
  StreamSubscription<BluetoothState>? _bluetoothStateSubscription;
  final MidiCommand _midiCommand = MidiCommand();

  @override
  void initState() {
    super.initState();

    _setupSubscription = _midiCommand.onMidiSetupChanged?.listen((data) async {
      setState(() {});
    });

    _bluetoothStateSubscription =
        _midiCommand.onBluetoothStateChanged.listen((data) {
      setState(() {});
    });
  }

  bool _didAskForBluetoothPermissions = false;

  @override
  void dispose() {
    _setupSubscription?.cancel();
    _bluetoothStateSubscription?.cancel();
    super.dispose();
  }

  IconData _deviceIconForType(String type) {
    switch (type) {
      case "native":
        return Icons.devices;
      case "network":
        return Icons.language;
      case "BLE":
        return Icons.bluetooth;
      default:
        return Icons.device_unknown;
    }
  }

  Future<void> _informUserAboutBluetoothPermissions(
      BuildContext context) async {
    if (_didAskForBluetoothPermissions) {
      return;
    }

    _didAskForBluetoothPermissions = true;

    return;
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Planter'),
          actions: <Widget>[
            Builder(builder: (context) {
              return IconButton(
                  onPressed: () async {
                    // Ask for bluetooth permissions
                    await _informUserAboutBluetoothPermissions(context);

                    // Start bluetooth
                    await _midiCommand.startBluetoothCentral();

                    await _midiCommand.waitUntilBluetoothIsInitialized();

                    // If bluetooth is powered on, start scanning
                    if (_midiCommand.bluetoothState ==
                        BluetoothState.poweredOn) {
                      _midiCommand
                          .startScanningForBluetoothDevices()
                          .catchError((err) {});
                      ScaffoldMessenger.of(context).showSnackBar(const SnackBar(
                        content: Text('Scanning for bluetooth devices ...'),
                      ));
                    } else {
                      final messages = {
                        BluetoothState.unsupported:
                            'Bluetooth is not supported on this device.',
                        BluetoothState.poweredOff:
                            'Please switch on bluetooth and try again.',
                        BluetoothState.poweredOn: 'Everything is fine.',
                        BluetoothState.resetting:
                            'Currently resetting. Try again later.',
                        BluetoothState.unauthorized:
                            'This app needs bluetooth permissions. Please open settings, find your app and assign bluetooth access rights and start your app again.',
                        BluetoothState.unknown:
                            'Bluetooth is not ready yet. Try again later.',
                        BluetoothState.other:
                            'This should never happen. Please inform the developer of your app.',
                      };
                      ScaffoldMessenger.of(context).showSnackBar(SnackBar(
                        backgroundColor: Colors.red,
                        content: Text(messages[_midiCommand.bluetoothState] ??
                            'Unknown bluetooth state: ${_midiCommand.bluetoothState}'),
                      ));
                    }
                    // If not show a message telling users what to do
                    setState(() {});
                  },
                  icon: const Icon(Icons.refresh));
            }),
          ],
        ),
        bottomNavigationBar: Container(
          padding: const EdgeInsets.all(24.0),
          child: const Text(
            "Tap to connnect/disconnect, long press to control.",
            textAlign: TextAlign.center,
          ),
        ),
        body: Center(
          child: FutureBuilder(
            future: _midiCommand.devices,
            builder: (BuildContext context, AsyncSnapshot snapshot) {
              if (snapshot.hasData && snapshot.data != null) {
                var devices = snapshot.data as List<MidiDevice>;
                return ListView.builder(
                  itemCount: devices.length,
                  itemBuilder: (context, index) {
                    MidiDevice device = devices[index];

                    return ListTile(
                      title: Text(
                        device.name,
                        style: Theme.of(context).textTheme.headline5,
                      ),
                      subtitle: Text(
                          "ins:${device.inputPorts.length} outs:${device.outputPorts.length}"),
                      leading: Icon(device.connected
                          ? Icons.radio_button_on
                          : Icons.radio_button_off),
                      trailing: Icon(_deviceIconForType(device.type)),
                      onLongPress: () {
                        _midiCommand.stopScanningForBluetoothDevices();
                        Navigator.of(context).push(MaterialPageRoute<void>(
                          builder: (_) => ControllerPage(device),
                        ));
                      },
                      onTap: () {
                        if (device.connected) {
                          _midiCommand.disconnectDevice(device);
                        } else {
                          _midiCommand.connectToDevice(device);
                        }
                      },
                    );
                  },
                );
              } else {
                return const CircularProgressIndicator();
              }
            },
          ),
        ),
      ),
    );
  }
}
