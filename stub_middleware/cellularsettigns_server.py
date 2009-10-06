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


Examples:

dbus-send --system --print-reply --dest=com.nokia.csd.SIM /com/nokia/csd/sim com.nokia.csd.SIM.GetSIMStatus


"""

import pygtk, dbus, dbus.service
import time

# this is needed or main loop is not binded by default
import dbus.glib

pygtk.require('2.0')
import gtk
import gtk.gdk
import gobject

bus = dbus.SystemBus()
#bus = dbus.SessionBus()

# /usr/include/csd/csd-csnet.h
bus_name = dbus.service.BusName("com.nokia.csd.CSNet", bus)
RADIO_PATH = "/com/nokia/csd/csnet/radio"
NETWORK_PATH = "/com/nokia/csd/csnet/network"

TIMEOUT = 1*1000

class WrongPassword(dbus.DBusException):
    _dbus_error_name = 'com.nokia.csd.SIM.Error.WrongPassword'


class ExtendedObject(dbus.service.Object):
    """ 
    Extended dbus service property to creates dbus properties
    for the introspection schema. For example:

    self.property('org.freedesktop.SampleInterface',
                  'Bar', 'y', 'readwrite')

       <interface name="org.freedesktop.SampleInterface">
           <property name="Bar" type="y" access="readwrite"/>
       </interface>
    """
    class DBusProp(object):
        def __init__(self, name, signature, access):
            self.name = name
            self.sign = signature
            self.access = access
        def __str__(self):
            return '    <property name="%s" type="%s" access="%s"/>\n' % \
                   (self.name, self.sign, self.access)

    def __init__(self):
        print 'inited'
        self.properties = {}

    def dbus_prop(self, iface, name, signature, access):
        if not iface in self.properties:
            self.properties[iface] = []
        self.properties[iface].append(self.DBusProp(name, signature, access))

    @dbus.service.method('org.freedesktop.DBus.Introspectable', in_signature='', out_signature='s')
    def Introspect(self):
        """Return a string of XML encoding this object's supported interfaces,
        methods and signals.
        """
        reflection_data = '<!DOCTYPE node PUBLIC "-//freedesktop//DTD D-BUS Object Introspection 1.0//EN" "http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd">\n'
        reflection_data += '<node name="%s">\n' % (self._object_path)

        interfaces = self._dbus_class_table[self.__class__.__module__ + '.' + self.__class__.__name__]
        for (name, funcs) in interfaces.iteritems():
            reflection_data += '  <interface name="%s">\n' % (name)

            for func in funcs.values():
                if getattr(func, '_dbus_is_method', False):
                    reflection_data += self.__class__._reflect_on_method(func)
                elif getattr(func, '_dbus_is_signal', False):
                    reflection_data += self.__class__._reflect_on_signal(func)
            if name in self.properties:
                for prop in self.properties[name]:
                    reflection_data += str(prop)
            reflection_data += '  </interface>\n'

        reflection_data += '</node>\n'

        return reflection_data

        
    def _message_cb(self, connection, message):
        #print '_message cb', message.get_member(), message.get_interface()
        return dbus.service.Object._message_cb(self, connection, message)


# https://dvcs.projects.maemo.org/git/?p=cellular/libcsnet;a=blob;f=csd-csnet/Radio.xml;h=87be1e5308712ffffb91b96ed5791bcf9b12fd95;hb=HEAD
class RadioServer(ExtendedObject):
    def __init__ (self, bus_name, path=RADIO_PATH):
        ExtendedObject.__init__(self)
        dbus.service.Object.__init__(self, bus_name, path)

        self.RADIO_MODES = [
            '', #Unknown',
            'Dual',
            'GSM',
            'UMTS'
            ]
        self.radio_mode_idx = 1

        self.dbus_prop('com.nokia.csd.CSNet.RadioAccess',
                       'SelectionMode', 's', 'read')


    @dbus.service.method("org.freedesktop.DBus.Properties",  out_signature='v')
    def Get(self, iface, key):
        if key == 'SelectionMode':
            return dbus.String(self.RADIO_MODES[self.radio_mode_idx], variant_level=0)
        else:
            print 'get property', key, typee

    @dbus.service.method('com.nokia.csd.CSNet.RadioAccess')
    def SetSelectionMode(self, mode):
        for i in range(len(self.RADIO_MODES)):
            if self.RADIO_MODES[i] == mode:
                self.radio_mode_idx = i
                self.SelectionModeChanged(self.RADIO_MODES[i])

    @dbus.service.signal(dbus_interface='com.nokia.csd.CSNet.RadioAccess', signature='s')
    def SelectionModeChanged(self, mode):
        print 'radio selection mode changed', mode
        self.on_selectionmodechanged(self, mode)
    def on_selectionmodechanged(self, mode): pass

class NetworkServer(ExtendedObject):
    def __init__ (self, bus_name, path=NETWORK_PATH):
        ExtendedObject.__init__(self)
        dbus.service.Object.__init__(self, bus_name, path)

        self.REGISTRATION_MODES = [
            '',
            'Auto',
            'Manual',
            ]
        self.registration_mode_idx = 2

        self.registration_fails = False

        self.dbus_prop('com.nokia.csd.CSNet.NetworkRegistration',
                       'SelectionMode', 's', 'read')

    @dbus.service.method("org.freedesktop.DBus.Properties",  out_signature='v')
    def Get(self, iface, key):
        if key == 'SelectionMode':
            return dbus.String(self.REGISTRATION_MODES[self.registration_mode_idx], variant_level=0)
        else:
            print 'get property', key, typee

    @dbus.service.method('com.nokia.csd.CSNet.NetworkRegistration')
    def SetSelectionMode(self, mode):
        for i in range(len(self.REGISTRATION_MODES)):
            if self.REGISTRATION_MODES[i] == mode:
                self.registration_mode_idx = i
                self.SelectionModeChanged(self.REGISTRATION_MODES[i])

    @dbus.service.signal(dbus_interface='com.nokia.csd.CSNet.NetworkRegistration', signature='s')
    def SelectionModeChanged(self, mode):
        print 'network selection mode changed', mode
        self.on_selectionmodechanged(self, mode)
    def on_selectionmodechanged(self, mode): pass


    @dbus.service.method('com.nokia.csd.CSNet.NetworkRegistration')
    def QueryNetworks(self):
        print 'query nets'
        time.sleep(2)

        #enum Availability
        # UnknownAvailability = -1, //!< Availability unknown
        # Available, //!< Operator is available
        # Current, //!< Operator is the current registered operator
        # NotAvailable //!< Operator is not available
        
        avails = [
            dbus.Struct([
            'Elisa','Available','Home','GSM_900_180','12','34',True ],
            signature='(ssssssb)'),
            dbus.Struct([
            'TDC','Available','Forbidden','AllSupported','12','34',False ],
            signature='(ssssssb)'),
            dbus.Struct([
            'Sonera','Current','Preferred','GSM_850_1900','12','34',True ],
            signature='(ssssssb)'),
        ]
        
        self.AvailableNetworks(avails)

    @dbus.service.signal(dbus_interface='com.nokia.csd.CSNet.NetworkRegistration', signature='s')
    def QueryFailed(self,message):
        pass
    
    @dbus.service.signal(dbus_interface='com.nokia.csd.CSNet.NetworkRegistration', signature='a(ssssssb)')
    def AvailableNetworks(self,avails):
        # arrays of:
        #, &name)
        #, &status) "Available","Current","Forbidden"
        #, &type) "Home",Preferred","Forbidden"
        #, &band) "AllSupported", "GSM_900_1800", "GSM_850_1900"
        #, &mnc)
        #, &mcc)
        #, &umts)
        print 'networks available', avails

    @dbus.service.method('com.nokia.csd.CSNet.NetworkRegistration', in_signature='uu')
    def Register(self, mcc, mnc):
        if self.registration_fails:
            self.RegisterFailed('no reason')
        else:
            self.Registered()

    @dbus.service.signal(dbus_interface='com.nokia.csd.CSNet.NetworkRegistration', signature='s')
    def RegisterFailed(self, message):
        print 'reg failed'
    
    @dbus.service.signal(dbus_interface='com.nokia.csd.CSNet.NetworkRegistration', signature='')
    def Registered(self):
        print 'reg'
        self.RegistrationChanged('asdf')

    @dbus.service.signal(dbus_interface='com.nokia.csd.CSNet.NetworkRegistration', signature='s')
    def RegistrationChanged(self, m):
        print 'reg chg'

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

        frame = gtk.Frame('Signals')
        main_box.add(frame)
        tmp = gtk.VBox()
        frame.add(tmp)
        frame = tmp

        frame.add(gtk.Label('Radio mode:'))
        rmode = gtk.combo_box_new_text()
        for m in self.radio_server.RADIO_MODES:
            rmode.append_text(m)
        frame.add(rmode)
        rmode.set_active(self.radio_server.radio_mode_idx)
        def cb_rmode_changed(widget):
            self.radio_server.radio_mode_idx = widget.get_active()
            self.radio_server.SetSelectionMode(
                self.radio_server.RADIO_MODES[self.radio_server.radio_mode_idx])
        rmode.cid = rmode.connect('changed', cb_rmode_changed)
        def on_selectionrmodechg(self, new_mode):
            for i in range(len(self.RADIO_MODES)):
                if self.RADIO_MODES[i] == new_mode:
                    rmode.disconnect(rmode.cid)
                    rmode.set_active(i)
                    rmode.cid = rmode.connect('changed', cb_rmode_changed) 
        self.radio_server.on_selectionmodechanged = on_selectionrmodechg

        frame.add(gtk.Label('Network registration mode:'))
        mode = gtk.combo_box_new_text()
        for m in self.network_server.REGISTRATION_MODES:
            mode.append_text(m)
        frame.add(mode)
        mode.set_active(self.network_server.registration_mode_idx)
        def cb_mode_changed(widget):
            print 'mode changed', widget.get_active()
            self.network_server.registration_mode_idx = widget.get_active()
            self.network_server.SetSelectionMode(
                self.network_server.REGISTRATION_MODES[self.network_server.registration_mode_idx])
        mode.cid = mode.connect('changed', cb_mode_changed) 
        def on_selectionmodechg(self, new_mode):
            for i in range(len(self.REGISTRATION_MODES)):
                if self.REGISTRATION_MODES[i] == new_mode:
                    mode.disconnect(mode.cid)
                    mode.set_active(i)
                    mode.cid = mode.connect('changed', cb_mode_changed) 
        self.network_server.on_selectionmodechanged = on_selectionmodechg


        # methods
        # =======

        registration = gtk.CheckButton('Registration fails')
        def callback(widget):
            self.network_server.registration_fails = widget.get_active()
        registration.connect('toggled', callback) 
        frame.add(registration)


        return main_box

    def __init__(self):
        self.radio_server = RadioServer(bus_name)
        self.network_server = NetworkServer(bus_name)
        print self.radio_server.Introspect()
        print self.network_server.Introspect()

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




