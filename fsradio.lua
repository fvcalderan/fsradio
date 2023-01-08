-- Thanks to thegeekforge.com for providing a working example of Arduino-FSUIPC
-- serial communication. The example can be found in the URL below.
-- http://thegeekforge.com/2017/11/10/arduino-lua-fsuipc-tutorial-001_a/

-- COM info
port_file = "fsradio_port.txt"
speed = 115200
handshake = 0

-- operation mode
mode = 0

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
function get_serial_data(com_port, datastr, length)
    -- Mode
    if (string.find(datastr, "MODE")) then
        mode = tonumber(string.sub(datastr, 6, string.len(datastr)))

    -- COM 1
    elseif (string.find(datastr, "COM1_MUP")) then
		ipc.control(65637, 0)   -- MHz Up
    elseif (string.find(datastr, "COM1_MDN")) then
		ipc.control(65636, 0)   -- MHz Down
    elseif (string.find(datastr, "COM1_KUP")) then
		ipc.control(65639, 0)   -- KHz Up
    elseif (string.find(datastr, "COM1_KDN")) then
		ipc.control(65638, 0)   -- KHz Down
    elseif (string.find(datastr, "COM1_SWP")) then
		ipc.control(66372, 0)   -- Standby Swap

    -- COM 2
    elseif (string.find(datastr, "COM2_MUP")) then
		ipc.control(66437, 0)   -- MHz Up
    elseif (string.find(datastr, "COM2_MDN")) then
		ipc.control(66436, 0)   -- MHz Down
    elseif (string.find(datastr, "COM2_KUP")) then
		ipc.control(66440, 0)   -- KHz Up
    elseif (string.find(datastr, "COM2_KDN")) then
		ipc.control(66438, 0)   -- KHz Down
    elseif (string.find(datastr, "COM2_SWP")) then
		ipc.control(66444, 0)   -- Standby Swap

    -- NAV 1
    elseif (string.find(datastr, "NAV1_MUP")) then
		ipc.control(65641, 0)   -- MHz Up
    elseif (string.find(datastr, "NAV1_MDN")) then
		ipc.control(65640, 0)   -- MHz Down
    elseif (string.find(datastr, "NAV1_KUP")) then
		ipc.control(65643, 0)   -- KHz Up
    elseif (string.find(datastr, "NAV1_KDN")) then
		ipc.control(65642, 0)   -- KHz Down
    elseif (string.find(datastr, "NAV1_SWP")) then
		ipc.control(66448, 0)   -- Standby Swap

    -- NAV 2
    elseif (string.find(datastr, "NAV2_MUP")) then
		ipc.control(65645, 0)   -- MHz Up
    elseif (string.find(datastr, "NAV2_MDN")) then
		ipc.control(65644, 0)   -- MHz Down
    elseif (string.find(datastr, "NAV2_KUP")) then
		ipc.control(65647, 0)   -- KHz Up
    elseif (string.find(datastr, "NAV2_KDN")) then
		ipc.control(65646, 0)   -- KHz Down
    elseif (string.find(datastr, "NAV2_SWP")) then
		ipc.control(66452, 0)   -- Standby Swap

    -- ADF
    elseif (string.find(datastr, "ADF_100UP")) then
		ipc.control(65648, 0)   -- 100 Up
    elseif (string.find(datastr, "ADF_100DN")) then
		ipc.control(65666, 0)   -- 100 Down
    elseif (string.find(datastr, "ADF_010UP")) then
		ipc.control(65649, 0)   -- 010 Up
    elseif (string.find(datastr, "ADF_010DN")) then
		ipc.control(65667, 0)   -- 010 Down
    elseif (string.find(datastr, "ADF_001UP")) then
		ipc.control(65650, 0)   -- 001 Up
    elseif (string.find(datastr, "ADF_001DN")) then
		ipc.control(65668, 0)   -- 001 Down
    elseif (string.find(datastr, "ADF_DECUP")) then
		ipc.control(66454, 0)   -- DEC Up
    elseif (string.find(datastr, "ADF_DECDN")) then
		ipc.control(66453, 0)   -- DEC Down

    -- DME
    elseif (string.find(datastr, "DME_TGL")) then
        ipc.control(65789 - ipc.readSD(0x378), 0) -- DME Toggle

    -- XPDR
    elseif (string.find(datastr, "XPDR_1000UP")) then
		ipc.control(65651, 0)   -- 1000 Up
    elseif (string.find(datastr, "XPDR_1000DN")) then
		ipc.control(66455, 0)   -- 1000 Down
    elseif (string.find(datastr, "XPDR_0100UP")) then
		ipc.control(65652, 0)   -- 0100 Up
    elseif (string.find(datastr, "XPDR_0100DN")) then
		ipc.control(66456, 0)   -- 0100 Down
    elseif (string.find(datastr, "XPDR_0010UP")) then
		ipc.control(65653, 0)   -- 0010 Up
    elseif (string.find(datastr, "XPDR_0010DN")) then
		ipc.control(66457, 0)   -- 0010 Down
    elseif (string.find(datastr, "XPDR_0001UP")) then
		ipc.control(65654, 0)   -- 0001 Up
    elseif (string.find(datastr, "XPDR_0001DN")) then
		ipc.control(66458, 0)   -- 0001 Down

    -- AP ALT
    elseif (string.find(datastr, "AP_ALT_UP")) then
		ipc.control(65892, 0)   -- Autopilot altitude Up
    elseif (string.find(datastr, "AP_ALT_DN")) then
		ipc.control(65893, 0)   -- Autopilot altitude Down
    elseif (string.find(datastr, "AP_ALT_H")) then
		ipc.control(65726, 0)   -- Autopilot altitude Hold

    -- AP VS
    elseif (string.find(datastr, "AP_VS_UP")) then
		ipc.control(65894, 0)   -- Autopilot vertical speed Up
    elseif (string.find(datastr, "AP_VS_DN")) then
		ipc.control(65895, 0)   -- Autopilot vertical speed Down

    -- AP HDG
    elseif (string.find(datastr, "AP_HDG_UP")) then
		ipc.control(65879, 0)   -- Autopilot heading Up
    elseif (string.find(datastr, "AP_HDG_DN")) then
		ipc.control(65880, 0)   -- Autopilot heading Down
    elseif (string.find(datastr, "AP_HDG_H")) then
		ipc.control(65725, 0)   -- Autopilot heading Hold

    -- AP NAV
    elseif (string.find(datastr, "AP_NAV_HLD")) then
		ipc.control(65729, 0)   -- Autopilot nav Hold

    -- AP APR
    elseif (string.find(datastr, "AP_APR_HLD")) then
		ipc.control(65724, 0)   -- Autopilot approach Hold

    -- AP NAV MOD
    elseif (string.find(datastr, "AP_NAV_MOD")) then
		ipc.control(66375, 0)   -- Autopilot nav mode Toggle

    -- AP MASTER
    elseif (string.find(datastr, "AP_MASTER")) then
		ipc.control(65580, 0)   -- Autopilot master Toggle

	end
end

-- Send data to Arduino
function set_serial_data()
    -- Choose which information to send

    -- COM 1
    if mode < 2 then -- "c1": update if coming from "c2"
        raw = string.sub(string.format("%x", ipc.readSD(0x311A)), -4)
        info = "c11"..string.sub(raw, 1, 2).."."..string.sub(raw, -2).."\n"

    -- COM 2
    elseif mode < 4 then -- "c2": update if coming from "c1"
        raw = string.sub(string.format("%x", ipc.readSD(0x311C)), -4)
        info = "c21"..string.sub(raw, 1, 2).."."..string.sub(raw, -2).."\n"

    -- NAV 1
    elseif mode < 6 then -- "n1": update if coming from "n2"
        raw = string.sub(string.format("%x", ipc.readSD(0x311E)), -4)
        info = "n11"..string.sub(raw, 1, 2).."."..string.sub(raw, -2).."\n"

    -- NAV 2
    elseif mode < 8 then -- "n2": update if coming from "n1"
        raw = string.sub(string.format("%x", ipc.readSD(0x3120)), -4)
        info = "n21"..string.sub(raw, 1, 2).."."..string.sub(raw, -2).."\n"

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
                                      -- 01234567
        info = dme..dist..knots.."\n" -- ABB.BCCC

    -- XPDR
    elseif mode < 17 then
        raw = string.format("%x", ipc.readSD(0x0354))
        info = string.sub(raw, -4).."\n"

    -- AP ALT
    elseif mode < 18 then
        raw = ipc.readSD(0x07D4)/19976
        round_nearest_100 = string.format("%05d", math.floor(raw/100+0.5)*100)
        if ipc.readSD(0x07D0) == 1 then hm = "HLD" else hm = "OFF" end
        info = round_nearest_100.." "..hm.."\n"

    -- AP VS
    elseif mode < 19 then
        raw = ipc.readSD(0x07F2)
        if raw >= 55636 then raw = raw - 65536 end -- deals with negative
        info = string.format("%05d", raw).."\n"

    -- AP HDG
    elseif mode < 20 then
        raw = math.floor(ipc.readSD(0x07CC)/182)
        if raw == 0 then raw = 360 end
        if ipc.readSD(0x07C8) == 1 then hm = "HLD" else hm = "OFF" end
        info = string.format("%03d", raw).." "..hm.."\n"

    -- AP NAV
    elseif mode < 21 then -- "n": update if coming from "a"
        if ipc.readSD(0x07C4) == 1 then hm = "nHLD" else hm = "nOFF" end
        info = hm.."\n"

    -- AP APR
    elseif mode < 22 then -- "a": update of coming from "n"
        if ipc.readSD(0x07FC) == 1 then hm = "aHLD" else hm = "aOFF" end
        info = hm.."\n"

    -- AP NAV MOD
    elseif mode < 23 then
        if ipc.readSD(0x132C) == 0 then hm = "NAV" else hm = "GPS" end
        info = hm.."\n"

    -- AP MASTER
    elseif mode < 24 then
        if ipc.readSD(0x07BC) == 0 then hm = "OFF" else hm = "ON" end
        info = hm.."\n"

    end

    -- Send information
    com.write(com_port, info)
end

-- Events
event.com(com_port, 50, 1, "get_serial_data")
event.timer(100, "set_serial_data")
