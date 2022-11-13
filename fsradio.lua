--http://thegeekforge.com/2017/11/10/arduino-lua-fsuipc-tutorial-001_a/

-- COM info
port_file = "fsradio_port.txt"
speed = 115200
handshake = 0

-- operation mode
mode = 0
MAX_MODE = 3

-- COM Port Setup
file = io.open(port_file, "r")

if file == nil then
	port_number = "10"
	file = io.open(port_file, "w")
	io.output(file)
	io.write(port_number)
	io.close(file)
	com_port = com.open("COM"..port_number, speed, handshake)
else
	port_number = file:read (2)
	io.close(file)
	com_port = com.open("COM"..port_number, speed, handshake)
end

if com_port ~= 0 then
	ipc.display("Arduino's COM Port "..port_number.." Open",5)
else
	ipc.display("Could not open Arduino's COM Port")
	ipc.sleep(2000)
	port_number = ipc.ask("\n\n\n\n\n\n\n\nEnter Arduino's COM Port:")
	file = io.open(port_file, "w")
	io.output(file)
	io.write(port_number)
	io.close(file)
	com_port = com.open("COM"..port_number, speed, handshake)

	if com_port == 0 then
		ipc.display("Could not open Arduino's COM Port",5)
		ipc.exit()
	else
		ipc.display("Arduino's COM Port "..port_number.." Open",5)
	end
end


-- Get data from Arduino
function get_serial_data(com_port, datastring, length)
    -- MODE
	if (string.find(datastring, "MODE_UP")) then
		mode = math.min(MAX_MODE, mode + 1) -- Mode Up
	end
	if (string.find(datastring, "MODE_DN")) then
		mode = math.max(0, mode - 1) -- Mode Down
	end

    -- COM 1
	if (string.find(datastring, "COM1_MUP")) then
		ipc.control(65637, 0)   -- MHz Up
	end
	if (string.find(datastring, "COM1_MDN")) then
		ipc.control(65636, 0)   -- MHz Down
	end
	if (string.find(datastring, "COM1_KUP")) then
		ipc.control(65639, 0)   -- KHz Up
	end
	if (string.find(datastring, "COM1_KDN")) then
		ipc.control(65638, 0)   -- KHz Down
	end
	if (string.find(datastring, "COM1_SWP")) then
		ipc.control(66372, 0)   -- Standby Swap
	end

    -- COM 2
	if (string.find(datastring, "COM2_MUP")) then
		ipc.control(66437, 0)   -- MHz Up
	end
	if (string.find(datastring, "COM2_MDN")) then
		ipc.control(66436, 0)   -- MHz Down
	end
	if (string.find(datastring, "COM2_KUP")) then
		ipc.control(66440, 0)   -- KHz Up
	end
	if (string.find(datastring, "COM2_KDN")) then
		ipc.control(66438, 0)   -- KHz Down
	end
	if (string.find(datastring, "COM2_SWP")) then
		ipc.control(66444, 0)   -- Standby Swap
	end

    ipc.display(datastring.." "..mode)
end

-- Send data to Arduino
function set_serial_data()
    -- Choose which information to send
    if mode < 2 then        -- COM 1
        raw = string.sub(string.format("%x", ipc.readSD(0x311A)), -4)
        info = "1"..string.sub(raw, 1, 2).."."..string.sub(raw, -2).."\n"
    elseif mode < 4 then    -- COM 2
        raw = string.sub(string.format("%x", ipc.readSD(0x311C)), -4)
        info = "1"..string.sub(raw, 1, 2).."."..string.sub(raw, -2).."\n"
    end
    -- Send information
    com.write(com_port, info)
end

-- Events
event.com(com_port, 50,1, "get_serial_data")
event.timer(100, "set_serial_data")
