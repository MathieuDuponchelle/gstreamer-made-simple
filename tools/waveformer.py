from gi.repository import GMS
from gi.repository import GLib

if __name__=="__main__":
    GMS.init()
    wf = GMS.Waveformer()
    wf.set_uri("file:///home/meh/Music/jackpot.ogg", True)
    loop = GLib.MainLoop()
    loop.run()
