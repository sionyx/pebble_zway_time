var STATUS = {
    AUTHENTICATE  : 0,
    LOADLOCATIONS : 1,
    LOADDEVICES   : 2,
    
    UNKNOWNERROR  : 100,
    NOCONNECTION  : 101,
    AUTHFAILED    : 102,
    NEEDSETUP     : 103
};

var ICONS = {
    UNKNOWN       : 0,
    'switch'      : 1,
    'humidity'    : 2,
    'temperature' : 3,
    'battery'     : 4,
    'motion'      : 5,
    'luminosity'  : 6
};

var URL = {
    LOGIN     : '',
    LOCATIONS : '',
    DEVICES   : '',
    COMMAND   : ''
};
var zway_session;

function getRequest(url, success, fail) {
    var xhr = new XMLHttpRequest();
    xhr.onload = function () {
        success(this.responseText);
    };
    xhr.onerror = function(e) {
        console.log('getRequest onerror');
        fail(e.error);
    };
    xhr.open('GET', url);
    xhr.setRequestHeader("ZWAYSession", zway_session);
    xhr.send();
};

function postRequest(url, payload, success, fail) {
    var xhr = new XMLHttpRequest();
    xhr.onload = function () {
        var status = this.status;
        var response = this.responseText;
        success(status, response);
    };
    xhr.onerror = function(e) {
        console.log('postRequest onerror');
        fail(e.error);
    };
    xhr.ontimeout = function() {
        console.log('postRequest timeout');
        fail('timeout');
    };
    xhr.open('POST', url);
    xhr.setRequestHeader('Content-Type', 'application/json');
    xhr.timeout = 3000;
    xhr.send(payload);
};

function zWayDevices() {
    Pebble.sendAppMessage({ 'KEY_STATUS' : STATUS.LOADDEVICES });
    
    function succesHandler(responseText) {
        var json = JSON.parse(responseText);
        if (json.code == 200) {
            Pebble.sendAppMessage({ 'KEY_DEVICES_START' : json.data.devices.length});
            
            json.data.devices.forEach(function logArrayElements(device, index, devices) {
                                      if (!device.location) {
                                      return;
                                      }
                                      
                                      var dictionary = {
                                      "KEY_DEVICE_TYPE"    : device.deviceType,
                                      "KEY_DEVICE_ID"      : device.id,
                                      "KEY_DEVICE_LOCATION": device.location,
                                      "KEY_DEVICE_LEVEL"   : device.metrics.level.toString() + (device.metrics.scaleTitle ? device.metrics.scaleTitle : ""),
                                      "KEY_DEVICE_TITLE"   : device.metrics.title,
                                      "KEY_DEVICE_ICON"    : ICONS[device.metrics.icon] || ICONS.UNKNOWN
                                      };
                                      Pebble.sendAppMessage(dictionary);
                                      });
            
            Pebble.sendAppMessage({ 'KEY_DEVICES_END' : 0});
        }
        else {
            Pebble.sendAppMessage({ 'KEY_STATUS' : STATUS.UNKNOWNERROR });
            console.log('z-way devices error');
        }
    }
    
    getRequest(URL.DEVICES, succesHandler);
}

function zWayLocations() {
    Pebble.sendAppMessage({ 'KEY_STATUS' : STATUS.LOADLOCATIONS });
    
    function succesHandler(responseText) {
        var json = JSON.parse(responseText);
        
        if (json.code == 200) {
            Pebble.sendAppMessage({ 'KEY_LOCATIONS_START' : json.data.length});
            
            json.data.forEach(function logArrayElements(location, index, locations) {
                              Pebble.sendAppMessage({
                                                    "KEY_LOCATION_TITLE": location.title,
                                                    "KEY_LOCATION_ID": location.id,
                                                    });
                              });
            
            Pebble.sendAppMessage({ 'KEY_LOCATIONS_END' : 0});
            
            zWayDevices();
        }
        else {
            Pebble.sendAppMessage({ 'KEY_STATUS' : STATUS.UNKNOWNERROR });
            console.log('z-way locations error');
        }
    }
    
    getRequest(URL.LOCATIONS, succesHandler);
}

function zWayLogin() {
    var zway_login = localStorage.getItem("zway_login");
    var zway_pass  = localStorage.getItem("zway_pass");
    
    if (typeof(zway_login) != 'string' || typeof(zway_pass) != 'string') {
        Pebble.sendAppMessage({ 'KEY_STATUS' : STATUS.NEEDSETUP });
        return;
    }
    
    Pebble.sendAppMessage({ 'KEY_STATUS' : STATUS.AUTHENTICATE });
    
    function succesHandler(status, responseText) {
        if (status != 200) {
            if (status == 401) {
                Pebble.sendAppMessage({ 'KEY_STATUS' : STATUS.AUTHFAILED });
            }
            else {
                Pebble.sendAppMessage({ 'KEY_STATUS' : STATUS.UNKNOWNERROR });
                console.log('z-way login status' + status);
            }
            return;
        }
        
        var json = JSON.parse(responseText);
        if (json.code == 200) {
            zway_session = json.data.sid;
            
            zWayLocations();
        }
        else {
            Pebble.sendAppMessage({ 'KEY_STATUS' : STATUS.UNKNOWNERROR });
            console.log('z-way login error' + json.code);
        }
    }
    function failHandler(error) {
        console.log('zWayLogin error ' + error);
        Pebble.sendAppMessage({ 'KEY_STATUS' : STATUS.NOCONNECTION });
    }
    
    var payload = JSON.stringify({
                                 "login" : zway_login,
                                 "password" : zway_pass,
                                 "form":"true",
                                 "keepme":"false",
                                 "default_ui":"1"
                                 });
    postRequest(URL.LOGIN, payload, succesHandler, failHandler);
}

function zWayCommand(deviceId, command) {
    var url = URL.COMMAND + deviceId + '/command/' + command;
    function succesHandler(responseText) {
        var json = JSON.parse(responseText);
        if (json.code != 200) {
            Pebble.sendAppMessage({ 'KEY_STATUS' : STATUS.UNKNOWNERROR });
            console.log('z-way command error message: ' + json.message);
        }
    }
    
    getRequest(url, succesHandler);
}

function updateUrls() {
    var zway_address = localStorage.getItem("zway_address");
    var zway_port    = localStorage.getItem("zway_port");
    
    if (typeof(zway_address) == 'string' && typeof(zway_port) == 'string') {
        var url_base = 'http://' + zway_address + ':' + zway_port + '/ZAutomation/api/v1';
        
        URL.LOGIN     = url_base + '/login';
        URL.LOCATIONS = url_base + '/locations';
        URL.DEVICES   = url_base + '/devices?since=0';
        URL.COMMAND   = url_base + '/devices/';
        
        return true;
    }
    
    return false;
}

function init() {
    if (updateUrls()) {
        zWayLogin();
    }
    else {
        Pebble.sendAppMessage({ 'KEY_STATUS' : STATUS.NEEDSETUP });
    }
}


function pebbleReadyHandler(e) {
    init();
}

function pebbleAppMessageHandler(e) {
    zWayCommand(e.payload.KEY_DEVICE_ID, e.payload.KEY_DEVICE_LEVEL);
}

function pebleConfigurationHandler() {
    var url = '';
    Pebble.openURL(url);
}

function pebbleWebviewClosedHandler(e) {
    var config_data = JSON.parse(decodeURIComponent(e.response));
    
    localStorage.setItem("zway_address", config_data['address_input']);
    localStorage.setItem("zway_port",    config_data['port_input']);
    localStorage.setItem("zway_login",   config_data['login_input']);
    localStorage.setItem("zway_pass",    config_data['pass_input']);
    
    init();
}

Pebble.addEventListener('ready', pebbleReadyHandler);
Pebble.addEventListener('appmessage', pebbleAppMessageHandler);
Pebble.addEventListener('showConfiguration', pebleConfigurationHandler);
Pebble.addEventListener('webviewclosed', pebbleWebviewClosedHandler);
