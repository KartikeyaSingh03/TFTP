import os

import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

class MainWindow(Gtk.Window):

    def __init__(self):
        Gtk.Window.__init__(self, title = 'TFTP Client')
        self.set_border_width(20)

        listbox = Gtk.Box(orientation = Gtk.Orientation.VERTICAL, spacing = 15)
        self.add(listbox)

        row_ip = Gtk.ListBoxRow()
        box_ip = Gtk.Box(orientation = Gtk.Orientation.HORIZONTAL, spacing = 100)
        row_ip.add(box_ip)

        label_ip = Gtk.Label(label = "IP Address")
        self.text_ip = Gtk.Entry()
        box_ip.pack_start(label_ip, True, True, 0)
        box_ip.pack_start(self.text_ip, True, True, 0)

        listbox.add(row_ip)

        row_port = Gtk.ListBoxRow()
        box_port = Gtk.Box(orientation = Gtk.Orientation.HORIZONTAL, spacing = 140)
        row_port.add(box_port)

        label_port = Gtk.Label(label = "Port")
        self.text_port = Gtk.Entry()
        box_port.pack_start(label_port, True, True, 0)
        box_port.pack_start(self.text_port, True, True, 0)

        listbox.add(row_port)

        row_file = Gtk.ListBoxRow()
        box_file = Gtk.Box(orientation = Gtk.Orientation.HORIZONTAL, spacing = 110)
        row_file.add(box_file)

        label_file = Gtk.Label(label = "Filename")
        self.text_file = Gtk.Entry()
        box_file.pack_start(label_file, True, True, 0)
        box_file.pack_start(self.text_file, True, True, 0)

        listbox.add(row_file)

        row_switch = Gtk.ListBoxRow()
        box_switch = Gtk.Box(orientation = Gtk.Orientation.HORIZONTAL, spacing = 80)
        
        self.button_put = Gtk.ToggleButton(label="PUT")
        self.button_put.set_active(False)
        self.button_put.connect("toggled", self.on_button_toggled, "PUT")
        box_switch.pack_start(self.button_put, True, True, 0)

        self.button_get = Gtk.ToggleButton(label="GET", use_underline=True)
        self.button_get.connect("toggled", self.on_button_toggled, "GET")
        box_switch.pack_start(self.button_get, True, True, 0)

        row_switch.add(box_switch)
        listbox.add(row_switch)

    def on_button_toggled(self, button, name):
        if button.get_active():
            state = "on"
            action = "-p"
            if name == "PUT":
                self.button_get.set_active(False)
            else:
                self.button_put.set_active(False)
                action = "-g"
            ip = self.text_ip.get_text()
            port = self.text_port.get_text()
            filename = self.text_file.get_text()
           
            command = "./client " + ip + " " + port + " " + action + " " + filename

            os.system("make clean")
            os.system("make")
            os.system(command)
            
        else:
            state = "off"
            if name == "PUT":
                self.button_get.set_active(True)
            else:
                self.button_put.set_active(True)
        print("Button", name, "was turned", state)

def main():
    window = MainWindow()
    window.connect("delete-event", Gtk.main_quit)
    window.show_all()

    Gtk.main()

if __name__ == '__main__':
    main()