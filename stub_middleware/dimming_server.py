"""
A graphical suite to test dbus bindings for MCE.


Written by Matti Katila

[in scracthbox]
dbus-daemon --session --print-address > /tmp/dbus-address &
export DBUS_SYSTEM_BUS_ADDRESS=`cat /tmp/dbus-address`
dbus-monitor --system

[run the server outside of scratchbox (because of broken pygtk package)]
export DBUS_SYSTEM_BUS_ADDRESS=`cat /tmp/dbus-address`
python dimming_server.py

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

from time import sleep

bus = dbus.SystemBus()
#bus = dbus.SessionBus()

bus_name = dbus.service.BusName('com.nokia.mce', bus)
PATH_SIG = '/com/nokia/mce/signal'
PATH_REQ = '/com/nokia/mce/request'

TIMEOUT = 1*1000

DISPLAY_MODES = [
            'on',
            'dimmed',
            'off',
            ]
global DISPLAY_MODE_IDX
DISPLAY_MODE_IDX = 2
#//LOCK (MCE)
#define SYS_MCE_TKMODE_GET                "get_tklock_mode"
#define SYS_MCE_DEVICELOCKMODE_GET        "get_devicelock_mode"

#define SYS_MCE_TSKBMODE_REQ              "req_tklock_mode_change"

#define SYS_MCE_DEVICELOCKMODE_SIG        "devicelock_mode_ind"
#define SYS_MCE_TKMODE_SIG                "tklock_mode_ind"

LOCK = 'unlocked'

class ServerReq(dbus.service.Object):
    def __init__ (self, bus_name, path=PATH_REQ):
        dbus.service.Object.__init__(self, bus_name, path)

    def on_getdisplaystatus(label): pass

    @dbus.service.method('com.nokia.mce.request')
    def get_display_status(self):
        global DISPLAY_MODE_IDX
        self.on_getdisplaystatus(self.getdisplaystatus)
        return DISPLAY_MODES[DISPLAY_MODE_IDX]

    @dbus.service.method('com.nokia.mce.request')
    def get_tklock_mode(self):
        return LOCK
    @dbus.service.method('com.nokia.mce.request')
    def req_tklock_mode_change(self, how):
        if how == LOCK:
            LOCK = 'locked'
        else:
            LOCK = 'unlocked'

class ServerSig(dbus.service.Object):
    def __init__ (self, bus_name, path=PATH_SIG):
        dbus.service.Object.__init__(self, bus_name, path)

    @dbus.service.signal(dbus_interface='com.nokia.mce.signal',
                         signature='s')
    def display_status_ind(self, status):
        print 'signal emit: display_status_ind:', status


    @dbus.service.signal(dbus_interface='com.nokia.mce.signal',
                         signature='si')
    def sig_hardware_key_state_ind(self, str, integer):
        print 'signal emit: hw key ind', str, integer

    @dbus.service.signal(dbus_interface='com.nokia.mce.signal',
                         signature='s')
    def tklock_mode_ind(self, state):
        LOCK = state
        print 'signal emit: touch screen lock mode:', state

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

        frame = gtk.Frame('MCE signals')
        main_box.add(frame)
        tmp = gtk.VBox()
        frame.add(tmp)
        frame = tmp
        

        mode = gtk.combo_box_new_text()
        g = None
        for m in DISPLAY_MODES:
            radio = gtk.RadioButton(group=g,label=m)
            if g == None:
                g = radio
            def cb_display(widget):
                if not widget.get_active(): return
                for i in range(len(DISPLAY_MODES)):
                    if DISPLAY_MODES[i] == widget.get_label():
                        global DISPLAY_MODE_IDX
                        DISPLAY_MODE_IDX = i
                        self.serverSig.display_status_ind(widget.get_label())
            radio.connect('toggled', cb_display) 

            frame.add(radio)

        g = None
        for m in 'locked unlocked'.split(' '):
            radio = gtk.RadioButton(group=g,label=m)
            if g == None:
                g = radio
            def cb_display(widget):
                if not widget.get_active(): return
                self.serverSig.tklock_mode_ind(widget.get_label())
            radio.connect('toggled', cb_display) 

            frame.add(radio)

        short = gtk.Button('Short power btn press')
        def cb_power(widget):
            # mce-dev -> mce-modes.h -> "up" / "down"
            # 0 might be power key code.
            self.serverSig.sig_hardware_key_state_ind('down', 0)
            sleep(1)
            self.serverSig.sig_hardware_key_state_ind('up', 0)
        short.connect('clicked', cb_power)
        frame.add(short)

        llong = gtk.Button('Long power btn press')
        def cb_power(widget):
            self.serverSig.sig_hardware_key_state_ind('down', 0)
            sleep(3)
            self.serverSig.sig_hardware_key_state_ind('up', 0)
        llong.connect('clicked', cb_power)
        frame.add(llong)

        pwr = gtk.Button('Power btn')
        def cb_pressed(widget):
            self.serverSig.sig_hardware_key_state_ind('down', 0)
        pwr.connect('pressed', cb_pressed)
        def cb_released(widget):
            self.serverSig.sig_hardware_key_state_ind('up', 0)
        pwr.connect('released', cb_released)
        frame.add(pwr)


        # methods
        # =======

        methods = gtk.Frame('Methods')
        main_box.add(methods)
        tmp = gtk.VBox()
        methods.add(tmp)
        methods = tmp

        # method get_display_status
        self.serverReq.getdisplaystatus = \
                  label = gtk.Label('get_display_status')
        methods.add(label)
        self.serverReq.on_getdisplaystatus = method_cb


        

        return main_box

    def __init__(self):
        self.serverSig = ServerSig(bus_name)
        self.serverReq = ServerReq(bus_name)

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




