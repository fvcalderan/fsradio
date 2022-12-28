--http://thegeekforge.com/2017/11/10/arduino-lua-fsuipc-tutorial-001_a/

-- COM info
port_file = "fsradio_port.txt"
speed = 115200
handshake = 0

-- operation mode
mode = 0
MAX_MODE = 16

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
    elseif (string.find(datastring, "MODE_DN")) then
		mode = math.max(0, mode - 1) -- Mode Down

    -- COM 1
    elseif (string.find(datastring, "COM1_MUP")) then
		ipc.control(65637, 0)   -- MHz Up
    elseif (string.find(datastring, "COM1_MDN")) then
		ipc.control(65636, 0)   -- MHz Down
    elseif (string.find(datastring, "COM1_KUP")) then
		ipc.control(65639, 0)   -- KHz Up
    elseif (string.find(datastring, "COM1_KDN")) then
		ipc.control(65638, 0)   -- KHz Down
    elseif (string.find(datastring, "COM1_SWP")) then
		ipc.control(66372, 0)   -- Standby Swap

    -- COM 2
    elseif (string.find(datastring, "COM2_MUP")) then
		ipc.control(66437, 0)   -- MHz Up
    elseif (string.find(datastring, "COM2_MDN")) then
		ipc.control(66436, 0)   -- MHz Down
    elseif (string.find(datastring, "COM2_KUP")) then
		ipc.control(66440, 0)   -- KHz Up
    elseif (string.find(datastring, "COM2_KDN")) then
		ipc.control(66438, 0)   -- KHz Down
    elseif (string.find(datastring, "COM2_SWP")) then
		ipc.control(66444, 0)   -- Standby Swap

    -- NAV 1
    elseif (string.find(datastring, "NAV1_MUP")) then
		ipc.control(65641, 0)   -- MHz Up
    elseif (string.find(datastring, "NAV1_MDN")) then
		ipc.control(65640, 0)   -- MHz Down
    elseif (string.find(datastring, "NAV1_KUP")) then
		ipc.control(65643, 0)   -- KHz Up
    elseif (string.find(datastring, "NAV1_KDN")) then
		ipc.control(65642, 0)   -- KHz Down
    elseif (string.find(datastring, "NAV1_SWP")) then
		ipc.control(66448, 0)   -- Standby Swap

    -- NAV 2
    elseif (string.find(datastring, "NAV2_MUP")) then
		ipc.control(65645, 0)   -- MHz Up
    elseif (string.find(datastring, "NAV2_MDN")) then
		ipc.control(65644, 0)   -- MHz Down
    elseif (string.find(datastring, "NAV2_KUP")) then
		ipc.control(65647, 0)   -- KHz Up
    elseif (string.find(datastring, "NAV2_KDN")) then
		ipc.control(65646, 0)   -- KHz Down
    elseif (string.find(datastring, "NAV2_SWP")) then
		ipc.control(66452, 0)   -- Standby Swap

    -- ADF
    elseif (string.find(datastring, "ADF_100UP")) then
		ipc.control(65648, 0)   -- 100 Up
    elseif (string.find(datastring, "ADF_100DN")) then
		ipc.control(65666, 0)   -- 100 Down
    elseif (string.find(datastring, "ADF_010UP")) then
		ipc.control(65649, 0)   -- 010 Up
    elseif (string.find(datastring, "ADF_010DN")) then
		ipc.control(65667, 0)   -- 010 Down
    elseif (string.find(datastring, "ADF_001UP")) then
		ipc.control(65650, 0)   -- 001 Up
    elseif (string.find(datastring, "ADF_001DN")) then
		ipc.control(65668, 0)   -- 001 Down
    elseif (string.find(datastring, "ADF_DECUP")) then
		ipc.control(66454, 0)   -- DEC Up
    elseif (string.find(datastring, "ADF_DECDN")) then
		ipc.control(66453, 0)   -- DEC Down

    -- DME
    elseif (string.find(datastring, "DME_TGL")) then
        ipc.control(65789 - ipc.readSD(0x378), 0) -- DME Toggle

    -- XPDR
    elseif (string.find(datastring, "XPDR_1000UP")) then
		ipc.control(65651, 0)   -- 1000 Up
    elseif (string.find(datastring, "XPDR_1000DN")) then
		ipc.control(66455, 0)   -- 1000 Down
    elseif (string.find(datastring, "XPDR_0100UP")) then
		ipc.control(65652, 0)   -- 0100 Up
    elseif (string.find(datastring, "XPDR_0100DN")) then
		ipc.control(66456, 0)   -- 0100 Down
    elseif (string.find(datastring, "XPDR_0010UP")) then
		ipc.control(65653, 0)   -- 0010 Up
    elseif (string.find(datastring, "XPDR_0010DN")) then
		ipc.control(66457, 0)   -- 0010 Down
    elseif (string.find(datastring, "XPDR_0001UP")) then
		ipc.control(65654, 0)   -- 0001 Up
    elseif (string.find(datastring, "XPDR_0001DN")) then
		ipc.control(66458, 0)   -- 0001 Down
	end

    -- ipc.display(datastring.." "..mode)
end

-- Send data to Arduino
function set_serial_data()
    -- Choose which information to send

    -- COM 1
    if mode < 2 then
        raw = string.sub(string.format("%x", ipc.readSD(0x311A)), -4)
        info = "1"..string.sub(raw, 1, 2).."."..string.sub(raw, -2).."\n"

    -- COM 2
    elseif mode < 4 then
        raw = string.sub(string.format("%x", ipc.readSD(0x311C)), -4)
        info = "1"..string.sub(raw, 1, 2).."."..string.sub(raw, -2).."\n"

    -- NAV 1
    elseif mode < 6 then
        raw = string.sub(string.format("%x", ipc.readSD(0x311E)), -4)
        info = "1"..string.sub(raw, 1, 2).."."..string.sub(raw, -2).."\n"

    -- NAV 2
    elseif mode < 8 then
        raw = string.sub(string.format("%x", ipc.readSD(0x3120)), -4)
        info = "1"..string.sub(raw, 1, 2).."."..string.sub(raw, -2).."\n"

    -- ADF
    elseif mode < 12 then
        raw = string.sub(string.format("%x", ipc.readSD(0x034C)), -3)
        raw_dec = string.sub(string.format("%x", ipc.readSD(0x0356)), -1)
        info = "0"..raw.."."..raw_dec.."\n"

    -- DME
    elseif mode < 13 then
        dme = ipc.readSD(0x0378)
        if dme == 1 then
            dist = string.format("%03.1f", ipc.readSW(0x0300)/10)
            knots = string.format("%03d", ipc.readSW(0x0302)/10)
        else
            dist = string.format("%03.1f", ipc.readSW(0x0306)/10)
            knots = string.format("%03d", ipc.readSW(0x0308)/10)
        end
        -- FIX CURSOR POSITION BUG ON MODE CHANGE
                                      -- 01234567
        info = dme..dist..knots.."\n" -- ABB.BCCC

    -- XPDR
    elseif mode < 17 then
        raw = string.format("%x", ipc.readSD(0x0354))
        info = string.sub(raw, -4).."\n"

    end

    -- ipc.display(info)

    -- Send information
    com.write(com_port, info)
end

-- Events
event.com(com_port, 50, 1, "get_serial_data")
event.timer(100, "set_serial_data")
