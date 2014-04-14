from gi.repository import GMS
from gi.repository import GLib
from gi.repository import Gst
from gi.repository import Gtk
import cairo

import signal

def draw_cb(da, cr, renderer):
    surf = renderer.get_surface()
    cr.set_source_surface(surf, 0, 0)
    cr.set_operator(cairo.OPERATOR_OVER)
    print "painting"
    cr.paint()
    return False

def configure_cb(da, event, renderer):
    renderer.props.width = da.get_allocated_width()
    renderer.props.height = da.get_allocated_height()
    return True

def redraw_cb(renderer, drawing_area):
    drawing_area.queue_draw()

if __name__=="__main__":
    GMS.init()
    wf = GMS.Waveformer()
    pipe = Gst.parse_launch("uridecodebin uri=file:///home/meh/Music/nice.mp4 \
            expose-all-streams=false caps=audio/x-raw ! \
            level interval=10000000 ! pulsesink sync=true")

    #pipe = Gst.parse_launch("pulsesrc ! \
    #        level interval=10000000 ! pulsesink sync=true")

    wf.set_uri("file:///home/meh/Music/nice.mp4", True)
    loop = GLib.MainLoop()

    renderer = GMS.CairoWaveformRenderer(waveformer=wf)

    da = Gtk.DrawingArea()
    da.connect("draw", draw_cb, renderer)
    da.connect("configure-event", configure_cb, renderer)

    renderer.connect("redraw", redraw_cb, da)

    da.set_size_request(500, 50)

    win = Gtk.Window()
    win.add(da)
    win.show_all()

    signal.signal(signal.SIGINT, signal.SIG_DFL)

    Gtk.main()

    pipe.set_state(Gst.State.NULL)
