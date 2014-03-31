from gi.repository import GMS
from gi.repository import GLib
from gi.repository import Gst

if __name__=="__main__":
    GMS.init()
    wf = GMS.Waveformer()
    pipe = Gst.parse_launch("uridecodebin uri=file:///home/meh/Music/nice.mp4 \
            expose-all-streams=false caps=audio/x-raw ! \
            level interval=10000000 ! pulsesink sync=true")

    #pipe = Gst.parse_launch("pulsesrc ! \
    #        level interval=10000000 ! pulsesink sync=true")

    wf.set_pipeline(pipe, True)
    pipe.set_state(Gst.State.PLAYING)
    loop = GLib.MainLoop()

    try:
        loop.run()
    except KeyboardInterrupt:
        pass

    pipe.set_state(Gst.State.NULL)
