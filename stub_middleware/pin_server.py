"""
A graphical suite to test dbus bindings for SIM.


Written by Matti Katila

[in scracthbox]
dbus-daemon --session --print-address > /tmp/dbus-address &
export DBUS_SYSTEM_BUS_ADDRESS=`cat /tmp/dbus-address`
dbus-monitor --system

[run the server outside of scratchbox (because of broken pygtk package)]
export DBUS_SYSTEM_BUS_ADDRESS=`cat /tmp/dbus-address`
python dbus_server.py


https://dvcs.projects.maemo.org/git/?p=cellular/libsim;a=blob;f=csd-sim/DBUS-INTERFACE;h=d9d58aeb0e0e1a723984a179690d63905e950f23;hb=HEAD

Examples:



dbus-send --system --print-reply --dest=com.nokia.csd.SIM /com/nokia/csd/sim com.nokia.csd.SIM.GetSIMStatus

dbus-send --system --print-reply --dest=com.nokia.csd.SIM /com/nokia/csd/sim com.nokia.csd.SIM.Identity.GetIMSI

dbus-send --system --print-reply --dest=com.nokia.csd.SIM /com/nokia/csd/sim com.nokia.csd.SIM.Identity.GetServiceProviderName

dbus-send --system --print-reply --dest=com.nokia.csd.SIM /com/nokia/csd/sim com.nokia.csd.SIM.Identity.GetHPLMN

dbus-send --system --print-reply --dest=com.nokia.csd.SIM /com/nokia/csd/sim com.nokia.csd.SIM.Security.VerifyPIN string:PIN string:1234

dbus-send --system --print-reply --dest=com.nokia.csd.SIM /com/nokia/csd/sim com.nokia.csd.SIM.Security.ChangePIN string:PIN string:1234 string:4321

dbus-send --system --print-reply --dest=com.nokia.csd.SIM /com/nokia/csd/sim com.nokia.csd.SIM.Security.DisablePINQuery string:PIN string:1234 

dbus-send --system --print-reply --dest=com.nokia.csd.SIM /com/nokia/csd/sim com.nokia.csd.SIM.Security.GetPINQueryState string:PIN

dbus-send --system --print-reply --dest=com.nokia.csd.SIM /com/nokia/csd/sim com.nokia.csd.SIM.Security.EnablePINQuery string:PIN string:1234 

dbus-send --system --print-reply --dest=com.nokia.csd.SIM /com/nokia/csd/sim com.nokia.csd.SIM.Security.VerifyPUK string:PUK string:11111111 string:1234

dbus-send --system --print-reply --dest=com.nokia.csd.SIM /com/nokia/csd/sim com.nokia.csd.SIM.Security.PINAttemptsLeft string:PIN

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

bus_name = dbus.service.BusName('com.nokia.csd.SIM', bus)
PATH = '/com/nokia/csd/sim'

# emergency
bus_name_emerg = dbus.service.BusName('com.nokia.csd.Call', bus)
PATH_emerg = '/com/nokia/csd/call'

TIMEOUT = 1*1000

class WrongPassword(dbus.DBusException):
    _dbus_error_name = 'com.nokia.csd.SIM.Error.WrongPassword'



class EmergencyServer(dbus.service.Object):
    def __init__ (self, bus_name, path=PATH_emerg):
        dbus.service.Object.__init__(self, bus_name, path)

    @dbus.service.method('com.nokia.csd.Call')
    def GetEmergencyNumbers(self):
        #self.on_getsimstatus(self.getsimstatus)
        return [ "112", "911" ]


class Server(dbus.service.Object):
    def __init__ (self, bus_name, path=PATH):
        dbus.service.Object.__init__(self, bus_name, path)

        self.Reset()

    
    @dbus.service.method('com.nokia.csd.SIM')
    def Reset(self):
        print 'reset'
        self.SIM_MODES = [
            'Unknown',
            'Ok',
            'NoSIM',
            'PermanentlyBlocked',
            'NotReady',
            'PINRequired',
            'PUKRequired',
            'Rejected',
            'SIMLockRejected',
            ]
        self.mode_idx = 0

        self.pin_attempts_left = 3
        self.puk_attempts_left = 10

        self.pin = '4321'

        return True

    @dbus.service.method('com.nokia.csd.SIM')
    def SetState(self, param):
        for idx in range(len(self.SIM_MODES)):
            if self.SIM_MODES[idx] == param:
                self.mode_idx = idx
                self.SIMStatus(self.SIM_MODES[idx])

    # interface: com.nokia.csd.SIM
    # ============================


    def on_getsimstatus(label): pass

    @dbus.service.method('com.nokia.csd.SIM')
    def GetSIMStatus(self):
        self.on_getsimstatus(self.getsimstatus)
        return self.SIM_MODES[self.mode_idx]

    @dbus.service.signal(dbus_interface='com.nokia.csd.SIM',
                         signature='s')
    def SIMStatus(self, status):
        print 'signal emit: SIMStatus:', status


    # interface: com.nokia.csd.SIM.Security
    # =====================================

    def on_verifypin(label): pass
    
    @dbus.service.method('com.nokia.csd.SIM.Security')
    def VerifyPIN(self, type, code):
        self.on_verifypin(self.verifypin)
        self.pin_attempts_left -= 1
        if code == self.pin:
	    if self.SIM_MODES[self.mode_idx] == 'PINRequired':
                self.pin_attempts_left = 3
                self.SetState('Ok')
            return True
        if self.pin_attempts_left == 0:
            self.SetState('PUKRequired')
        raise WrongPassword('')

    def on_verifypuk(label): pass
    
    @dbus.service.method('com.nokia.csd.SIM.Security')
    def VerifyPUK(self, type, code, newpin):
        self.on_verifypuk(self.verifypuk)

        self.puk_attempts_left -= 1
        if code == '007':
            self.pin = newpin
	    if self.SIM_MODES[self.mode_idx] == 'PUKRequired':
                self.puk_attempts_left = 10
                self.SetState('Ok')
            return True
        if self.puk_attempts_left == 0:
            self.SetState('PermanentlyBlocked')
        raise WrongPassword('')

    @dbus.service.signal(dbus_interface='com.nokia.csd.SIM.Security',
                         signature='s')
    def VerifyCodeRequired(self, status):
        print 'signal emit: VerifyCodeRequired:', status


    def on_pinattemtpsleft(label): pass

    @dbus.service.method('com.nokia.csd.SIM.Security')
    def PINAttemptsLeft(self, type):
        self.on_pinattemptsleft(self.pinattemptsleft)
        if not self.SIM_MODES[self.mode_idx] == 'PINRequired':
            return -1
        return self.pin_attempts_left

    def on_pukattemptsleft(label): pass

    @dbus.service.method('com.nokia.csd.SIM.Security')
    def PUKAttemptsLeft(self, type):
        self.on_pukattemptsleft(self.pukattemptsleft)
        if not self.SIM_MODES[self.mode_idx] == 'PUKRequired':
            return -1
        return self.puk_attempts_left

    @dbus.service.method('com.nokia.csd.SIM.Security')
    def ChangePIN(self, type, old_pin, new_pin):
        self.on_changepin(self.changepin)
        if self.pin == old_pin:
            self.pin = new_pin
            return True
        raise WrongPassword('')


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

    def hello(self, widget, data):
        print 'hello'

    def create_widgets(self):
        main_box = gtk.VBox()


        # signals
        # =======

        frame = gtk.Frame('SIM signals')
        main_box.add(frame)
        tmp = gtk.VBox()
        frame.add(tmp)
        frame = tmp
        
        liststore = gtk.ListStore(gobject.TYPE_STRING)
        mode = gtk.combo_box_new_text()
        for m in self.server.SIM_MODES:
            mode.append_text(m)
        frame.add(mode)
        mode.set_active(self.server.mode_idx)
        def cb_sim_mode_changed(widget):
            print 'mode changed', widget.get_active()
            self.server.mode_idx = widget.get_active()
            self.server.SIMStatus(
                self.server.SIM_MODES[self.server.mode_idx])
        mode.connect('changed', cb_sim_mode_changed) 

        verify = gtk.Button('Verify code required')
        def cb_verify_code_req(widget):
            self.server.VerifyCodeRequired('PIN')
        verify.connect('clicked', cb_verify_code_req)
        frame.add(verify)


        # methods
        # =======

        methods = gtk.Frame('Methods')
        main_box.add(methods)
        tmp = gtk.VBox()
        methods.add(tmp)
        methods = tmp

        # method GetSIMStatus
        self.server.getsimstatus = label = gtk.Label('GetSIMStatus')
        methods.add(label)
        self.server.on_getsimstatus = method_cb

        # method sec.VerifyPIN
        self.server.verifypin = label = gtk.Label('VerifyPIN')
        methods.add(label)
        self.server.on_verifypin = method_cb

        # method sec.VerifyPUK
        self.server.verifypuk = label = gtk.Label('VerifyPUK')
        methods.add(label)
        self.server.on_verifypuk = method_cb

        # method sec.PINAttemptsLeft
        self.server.pinattemptsleft = label = gtk.Label('PINAttemptsLeft')
        methods.add(label)
        self.server.on_pinattemptsleft = method_cb

        # method sec.PUKAttemptsLeft
        self.server.pukattemptsleft = label = gtk.Label('PUKAttemptsLeft')
        methods.add(label)
        self.server.on_pukattemptsleft = method_cb

        # method sec.ChangePIN
        self.server.changepin = label = gtk.Label('ChangePIN')
        methods.add(label)
        self.server.on_changepin = method_cb

        

        return main_box

    def __init__(self):
        self.server = Server(bus_name)
        self.server_emerg = EmergencyServer(bus_name_emerg)

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




