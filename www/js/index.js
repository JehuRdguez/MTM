
var app = {
    initialize: function () {
        this.bindEvents();
        this.showMainPage();
    },
    bindEvents: function () {

        var TOUCH_START = 'touchstart';
        if (window.navigator.msPointerEnabled) { // windows phone
            TOUCH_START = 'MSPointerDown';
        }
        document.addEventListener('deviceready', this.onDeviceReady, false);
        refreshButton.addEventListener(TOUCH_START, this.refreshDeviceList, false);
        disconnectButton.addEventListener(TOUCH_START, this.disconnect, false);
        deviceList.addEventListener('touchstart', this.connect, false);
    },
    onDeviceReady: function () {
        app.refreshDeviceList();
    },
    refreshDeviceList: function () {
        bluetoothSerial.list(app.onDeviceList, app.onError);
    },
    onDeviceList: function (devices) {
        var option;

        // remove existing devices
        deviceList.innerHTML = "";
        app.setStatus("");

        devices.forEach(function (device) {

            listItem = document.createElement('li');
            listItem.className = "topcoat-list__item";
            html = '<b>' + device.name + '</b><br/>' + device.id;

            listItem.innerHTML = html;

            if (cordova.platformId === 'windowsphone') {
                // This is a temporary hack until I get the list tap working
                var button = document.createElement('button');
                button.innerHTML = "Connect";
                button.addEventListener('click', app.connect, false);
                button.dataset = {};
                button.dataset.deviceId = device.id;
                listItem.appendChild(button);
            } else {
                listItem.dataset.deviceId = device.id;
            }
            deviceList.appendChild(listItem);
        });

        if (devices.length === 0) {

            option = document.createElement('option');
            option.innerHTML = "No Bluetooth Devices";
            deviceList.appendChild(option);

            if (cordova.platformId === "ios") { // BLE
                app.setStatus("No se encontraron dispositivos Bluetooth.");
            } else { // Android or Windows Phone
                app.setStatus("Conecte un dispositivo Bluetooth");
            }

        } else {
            app.setStatus("Se encontraron " + devices.length + " dispositivo" + (devices.length === 1 ? "." : "s."));
        }

    },
    connect: function (e) {
        var onConnect = function () {
            // subscribe for incoming data
            bluetoothSerial.subscribe('\n', app.onData, app.onError);

            gasDiv.innerHTML = "  ";
            app.setStatus("Conectado");
            app.showDetailPage();
        };

        var deviceId = e.target.dataset.deviceId;
        if (!deviceId) { // try the parent
            deviceId = e.target.parentNode.dataset.deviceId;
        }

        conexion = bluetoothSerial.connect(deviceId, onConnect, app.onError);
    },
    onData: function (data) { // data received from Arduino
        console.log(data);

        const splitString = data.split(" ");
        gasDiv.innerHTML = splitString[0];

        ultrasonicoDiv.innerHTML = +splitString[2] + "cm";

        tempDiv.innerHTML = +splitString[6] + "°C";
        humDiv.innerHTML = +splitString[4] + "%";

        if (splitString[8] < 100) {
            fuegoDiv.innerHTML = "PELIGRO, INCENDIO DETECTADO";
        } else {
            fuegoDiv.innerHTML = "Sin fuego = " + splitString[8];
        }

        fotorresistenciaDiv.innerHTML = +splitString[10];

    },
    sendToArduino: function (op) {
        bluetoothSerial.write(op + "\n");
    },
    disconnect: function (event) {
        bluetoothSerial.disconnect(app.showMainPage, app.onError);
    },
    showMainPage: function () {
        mainPage.style.display = "block"; //block
        detailPage.style.display = "none"; //none
        pantallaGas.style.display = "none";
        pantallaProximidad.style.display = "none";
        pantallaTemp.style.display = "none";
        pantallaFlama.style.display = "none";
        pantallaLuz.style.display = "none";


    },
    showDetailPage: function () {
        mainPage.style.display = "none";
        detailPage.style.display = "block";
        pantallaGas.style.display = "none";
        pantallaProximidad.style.display = "none";
        pantallaTemp.style.display = "none";
        pantallaFlama.style.display = "none";
        pantallaLuz.style.display = "none";
    },
    ShowModuloGas: function () {
        mainPage.style.display = "none";
        detailPage.style.display = "none";
        pantallaGas.style.display = "block";
        pantallaProximidad.style.display = "none";
        pantallaTemp.style.display = "none";
        pantallaFlama.style.display = "none";
        pantallaLuz.style.display = "none";
    },
    ShowModuloProximidad: function () {
        mainPage.style.display = "none";
        detailPage.style.display = "none";
        pantallaGas.style.display = "none";
        pantallaProximidad.style.display = "block";
        pantallaTemp.style.display = "none";
        pantallaFlama.style.display = "none";
        pantallaLuz.style.display = "none";
    },
    ShowModuloTemp: function () {
        mainPage.style.display = "none";
        detailPage.style.display = "none";
        pantallaGas.style.display = "none";
        pantallaProximidad.style.display = "none";
        pantallaTemp.style.display = "block";
        pantallaFlama.style.display = "none";
        pantallaLuz.style.display = "none";
    },
    ShowModuloFlama: function () {
        mainPage.style.display = "none";
        detailPage.style.display = "none";
        pantallaGas.style.display = "none";
        pantallaProximidad.style.display = "none";
        pantallaTemp.style.display = "none";
        pantallaFlama.style.display = "block";
        pantallaLuz.style.display = "none";
    },
    ShowModuloLuz: function () {
        mainPage.style.display = "none";
        detailPage.style.display = "none";
        pantallaGas.style.display = "none";
        pantallaProximidad.style.display = "none";
        pantallaTemp.style.display = "none";
        pantallaFlama.style.display = "none";
        pantallaLuz.style.display = "block";
    },
    setStatus: function (message) {
        console.log(message);

        window.clearTimeout(app.statusTimeout);
        statusDiv.innerHTML = message;
        statusDiv.className = 'fadein';

        // automatically clear the status with a timer
        app.statusTimeout = setTimeout(function () {
            statusDiv.className = 'fadeout';
        }, 5000);
    },
    onError: function () {
        alert("ERROR: No se puede conectar con el dispositivo, verifica la conexión"); // real apps should use notification.alert
    }
};

$("#boton1").click(function () {
    app.sendToArduino("1");
});

$("#boton2").click(function () {
    app.sendToArduino("2");
});

$("#boton3").click(function () {
    app.sendToArduino("3");
});

$("#boton4").click(function () {
    app.sendToArduino("4");
});

$("#boton5").click(function () {
    app.sendToArduino("5");
});

$("#boton6").click(function () {
    app.sendToArduino("6");
});

$("#boton7").click(function () {
    app.sendToArduino("7");
});

$("#boton8").click(function () {
    app.sendToArduino("8");
});



$("#moduloGas").click(function () {
    app.ShowModuloGas();
});

$("#moduloUltra").click(function () {
    app.ShowModuloProximidad();
});

$("#moduloTemp").click(function () {
    app.ShowModuloTemp();
});

$("#moduloFuego").click(function () {
    app.ShowModuloFlama();
});

$("#moduloLuz").click(function () {
    app.ShowModuloLuz();
});

$("#inicio").click(function () {
    app.showDetailPage();
});

$("#inicio2").click(function () {
    app.showDetailPage();
});

$("#inicio3").click(function () {
    app.showDetailPage();
});

$("#inicio4").click(function () {
    app.showDetailPage();
});

$("#inicio5").click(function () {
    app.showDetailPage();
});



$("#LuzOffBtn").click(function () {
    app.sendToArduino("b");
});

$("#LuzOnBtn").click(function () {
    app.sendToArduino("a");
});


$("#GasOffBtn").click(function () {
    app.sendToArduino("d");
});

$("#GasOnBtn").click(function () {
    app.sendToArduino("c");
});
