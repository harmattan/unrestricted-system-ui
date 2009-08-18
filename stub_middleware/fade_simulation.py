
import os, sys, time, gtk, Image, StringIO

import pygtk
pygtk.require('2.0')
import gtk

# http://snippets.dzone.com/posts/show/655
def image2pixbuf(im):  
    file1 = StringIO.StringIO()  
    im.save(file1, "ppm")  
    contents = file1.getvalue()  
    file1.close()  
    loader = gtk.gdk.PixbufLoader("pnm")  
    loader.write(contents, len(contents))  
    pixbuf = loader.get_pixbuf()  
    loader.close()  
    return pixbuf 


def grab_screen():
  # http://github.com/Fiona/pycatcher/blob/4ea70e305fab9bd46e7dead0065640c05ccf0093/pycatcher/main.py
  try:
 
    img_width = gtk.gdk.screen_width()
    img_height = gtk.gdk.screen_height()
    
    screengrab = gtk.gdk.Pixbuf(
      gtk.gdk.COLORSPACE_RGB,
      False,
      8,
      img_width,
      img_height
    )
 
    screengrab.get_from_drawable(
      gtk.gdk.get_default_root_window(),
      gtk.gdk.colormap_get_system(),
      0, 0, 0, 0,
      img_width,
      img_height
    )
 
  except:
     die("Failed taking screenshot")
 
  print "Converting to PIL image ..."
 
  final_screengrab = Image.frombuffer(
    "RGB",
    (img_width, img_height),
    screengrab.get_pixels(),
    "raw",
    "RGB",
    screengrab.get_rowstride(),
    1
  )
  return final_screengrab


def brightness(img, val):
  import ImageEnhance

  enhancer = ImageEnhance.Brightness(img)
  return enhancer.enhance(val)




if __name__ == '__main__':
  grab = grab_screen()

  w = gtk.Window(gtk.WINDOW_TOPLEVEL)
  events = gtk.EventBox()
  def button_released(widget, event):
      print 'BUTTON RELEASED'
  events.connect('button-release-event', button_released)
  img = gtk.Image()
  img.set_from_pixbuf(image2pixbuf(grab))
  events.add(img)
  w.add(events)
  w.show_all()

  valueT = [ 0 ]
  def callback():
      valueT[0] += 1
      value = valueT[0] 

      if value % 4 == 0:
          dimm = brightness(grab, 1)
      elif value % 4 == 2:
          dimm = brightness(grab, 0)
      else:
          dimm = brightness(grab, 0.5)
      img.set_from_pixbuf(image2pixbuf(dimm))
      return True

  import glib
  #glib.timeout_add(1*1000, callback)



  def displayStateChanged(state):
      print 'changed:',state
      bright = 0
      if state == 'on': bright = 1
      elif state == 'dimmed': bright = 0.5
      dimm = brightness(grab, bright)
      img.set_from_pixbuf(image2pixbuf(dimm))

  import gobject
  import dbus
  from dbus.mainloop.glib import DBusGMainLoop
  dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

  bus = dbus.SystemBus()
  bus.add_signal_receiver(displayStateChanged,
                          #bus_name="com.nokia.mce",
                          path="/com/nokia/mce/signal",
                          dbus_interface="com.nokia.mce.signal",
                          signal_name="display_status_ind")

  mce = bus.get_object("com.nokia.mce", "/com/nokia/mce/request")
  displayStateChanged(mce.get_display_status())

  gtk.main()

