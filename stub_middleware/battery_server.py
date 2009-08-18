"""
A graphical suite to test dbus bindings for MCE.


Written by Matti Katila

[in scracthbox]
dbus-daemon --session --print-address > /tmp/dbus-address &
export DBUS_SYSTEM_BUS_ADDRESS=`cat /tmp/dbus-address`
dbus-monitor --system

[run the server outside of scratchbox (because of broken pygtk package)]
export DBUS_SYSTEM_BUS_ADDRESS=`cat /tmp/dbus-address`
python dbus_server.py

https://projects.maemo.org/svn/dsm/QtAPI/trunk/system/msystemdbus.h


Examples:



dbus-send --system --print-reply --dest=com.nokia.csd.SIM /com/nokia/csd/sim com.nokia.csd.SIM.Security.PUKAttemptsLeft string:PIN


"""

import pygtk, dbus, dbus.service

# this is needed or main loop is not binded by default
import dbus.glib

pygtk.require('2.0')
import gtk
import gtk.gdk
import gobject

bus = dbus.SystemBus()
#bus = dbus.SessionBus()

bus_name_sig = dbus.service.BusName('com.nokia.bme', bus)
PATH_SIG = '/com/nokia/bme/signal'

bus_name_req = dbus.service.BusName('org.freedesktop.Hal', bus)
PATH_REQ = '/org/freedesktop/Hal/devices/bme'

TIMEOUT = 1*1000

class ServerObj:
    def __init__(self):
        self.LEVELS = [
            'Full', 'Low', 'Critical'
            ]
        self.level_idx = 0

        self.STATES = [
            'Charging', 'NotCharging'
            ]
        self.state_idx = 0

        self.charge_level_percentage = 90
    

class ServerReq(dbus.service.Object):
    def __init__ (self, bus_name, server, path=PATH_REQ):
        dbus.service.Object.__init__(self, bus_name, path)
        self.obj = server

    #def on_getdisplaystatus(label): pass

    @dbus.service.method('org.freedesktop.Hal.Device')
    def GetProperty(self, key):
        if key == 'battery.charge_level.percentage':
            print 'percentage query'
            return dbus.Int32(self.obj.charge_level_percentage, variant_level=0)
        elif key == 'battery.rechargeable.is_charging':
            print 'is charging?'
            return dbus.Boolean(self.obj.state_idx==0, variant_level=0)
            #dbus.Struct(('', 42), signature='su')
            #return 1 - self.obj.state_idx
	else:
	    print 'nothing'
class ServerSig(dbus.service.Object):
    def __init__ (self, bus_name, server, path=PATH_SIG):
        dbus.service.Object.__init__(self, bus_name, path)
        self.obj = server

    # battery level
    @dbus.service.signal(dbus_interface='com.nokia.bme.signal', signature='')
    def battery_full(self):
        print 'signal emit: battery full'
        self.obj.charge_level_percentage = 98
    @dbus.service.signal(dbus_interface='com.nokia.bme.signal', signature='')
    def battery_low(self):
        print 'signal emit: battery low'
        self.obj.charge_level_percentage = 23
    @dbus.service.signal(dbus_interface='com.nokia.bme.signal', signature='')
    def battery_empty(self):
        print 'signal emit: battery empty'
        self.obj.charge_level_percentage = 5

    # charger connection state
    @dbus.service.signal(dbus_interface='com.nokia.bme.signal', signature='')
    def charger_connected(self):
        print 'signal emit: charger connected'
    @dbus.service.signal(dbus_interface='com.nokia.bme.signal', signature='')
    def charger_disconnected(self):
        print 'signal emit: charger disconnected'


def set_text_color(widget, color):
    widget.modify_fg (gtk.STATE_NORMAL ,
                      gtk.gdk.color_parse(color))
    
def method_cb(label):
    set_text_color(label, "red")
    def unset(label):
        set_text_color(label, "black")
        return False
    gobject.timeout_add(TIMEOUT, unset, label)


class UserInterface:
    def delete_event(self, widget, event, data=None):
        return False

    def destroy(self, widget, data=None):
        print "destroy signal occurred"
        gtk.main_quit()

    def create_widgets(self):
        main_box = gtk.VBox()

        # signals
        # =======

        frame = gtk.Frame('BME signals')
        main_box.add(frame)
        tmp = gtk.VBox()
        frame.add(tmp)
        frame = tmp
        

        level = gtk.combo_box_new_text()
        for m in self.serv_obj.LEVELS:
            level.append_text(m)
        frame.add(level)
        level.set_active(self.serv_obj.level_idx)
        def cb_level_changed(widget):
            print 'level changed', widget.get_active()
            idx = self.serv_obj.level_idx = widget.get_active()
            if idx == 0: self.serverSig.battery_full()
            elif idx == 1: self.serverSig.battery_low()
            else: self.serverSig.battery_empty()
        level.connect('changed', cb_level_changed) 
        
        state = gtk.combo_box_new_text()
        for m in self.serv_obj.STATES:
            state.append_text(m)
        frame.add(state)
        state.set_active(self.serv_obj.state_idx)
        def cb_state_changed(widget):
            print 'state changed', widget.get_active()
            idx = self.serv_obj.state_idx = widget.get_active()
            if idx == 0: self.serverSig.charger_connected()
            else: self.serverSig.charger_disconnected()
        state.connect('changed', cb_state_changed) 



        # methods
        # =======

        methods = gtk.Frame('Methods')
        main_box.add(methods)
        tmp = gtk.VBox()
        methods.add(tmp)
        methods = tmp

        # method get_display_status
        #self.serverReq.getdisplaystatus = \
        #          label = gtk.Label('get_display_status')
        #methods.add(label)
        #self.serverReq.on_getdisplaystatus = method_cb

        return main_box

    def __init__(self):
        self.serv_obj = ServerObj()
        self.serverSig = ServerSig(bus_name_sig, self.serv_obj)
        self.serverReq = ServerReq(bus_name_req, self.serv_obj)

        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.connect("delete_event", self.delete_event)
        self.window.connect("destroy", self.destroy)
        self.window.set_border_width(30)

        self.window.add(self.create_widgets())
    
        self.window.show_all()

    def main(self):
        gtk.main()

if __name__ == "__main__":
    ui = UserInterface()
    ui.main()




