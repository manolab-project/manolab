
// LABEL LAYERS
// =====================================================

/**
 * type: supported tyepes are: image, qrcode, text
 * name: layer name, displayed on the tool, not used for the label
 * value: depends on the type:
 *    - images: path of the image (full or relative to executabe)
 *    - qrcode: text to encode
 *    - text: text to display
 *
 * param1: option for the layer, depends on the type:
 *    - text: specify the font, point size, italic ... comma-separated list of the font attributes with the following format:
 *           font family,    pixelSize,     Weight [0 99],   Italic (0,1),   underline (0,1),  strikeOut (0,1), alignement (0=center 1=left, 2=right) 
 *            example: "Myriad Pro, 60, 75, 0, 0, 0, 0"
 */


var background = {
	type: "image",
	name: "Fond étiquette",
	value: "D:/git/noveamap/src/background.bmp",
	param1: "",
	x:0,
	y:0,
	width: 626,
	height: 744
};

var qrcode = {
	type: "qrcode",
	name: "QR code",
	value: "BAD_VALUE",
	param1: "",
	x:50,
	y:110,
	width: 120,
	height: 120
};

var title = {
	type: "text",
	name: "Title + serial",
	value: "NOVBOX",
	//      font family,    pixelSize,     Weight [0 99],   Italic (0,1),   underline (0,1),  strikeOut (0,1), alignement (0=center 1=left, 2=right) 
	param1: "Myriad Pro,       60,             75,              0,               0,                0,                  0",
	x:30,
	y:20,
	width: 570,
	height: 60
};

var battery_voltage = {
	type: "text",
	name: "Battery voltage",
	value: "12.8 V",
	//      font family,    pixelSize,     Weight [0 99],   Italic (0,1),   underline (0,1),  strikeOut (0,1), alignement (0=center 1=left, 2=right) 
	param1: "Myriad Pro,       60,             20,              0,               0,                0,                  0",
	x:180,
	y:160,
	width: 180,
	height: 60
};

var battery_capacity = {
	type: "text",
	name: "Battery capacity",
	value: "819 Wh",
	//      font family,    pixelSize,     Weight [0 99],   Italic (0,1),   underline (0,1),  strikeOut (0,1), alignement (0=center 1=left, 2=right) 
	param1: "Myriad Pro,       60,             75,              0,               0,                0,                  0",
	x:50,
	y:240,
	width: 310,
	height: 60
};

// API FUNCTIONS
// =====================================================

/**
 * @brief Called when a print order is set
 */
function print_label(png_image_hex, image_size)
{
	// Generate ZPL image data
	var label_data = "^XA^MNY^LL300~DYR:NOVPROD,P,P," + parseInt(image_size) + ",," + png_image_hex + "^XZ";
	writeToFile("label.zpl", label_data);
	
	// Generate separated print order command
	var printit_data = "^XA^FO0,0^XGR:NOVPROD.PNG,1,1^FS^XZ";
	writeToFile("printit.zpl", printit_data);
	
	systemPrint(executeCommand("lp -o raw label.zpl"));
	systemPrint(executeCommand("lp -o raw printit.zpl"));
	
	mapping[21] = 0; // reset print order
}


/**
 * @brief This function is called whenever the Modbus mapping is accessed in write only

width: width of the label
height: height of the label
dpi: dpi of the label
printer: select how to use the printer. Possible options are:
     * "default": use the operating system default printer (using Qt Printer facility)
	 * "command": use the command line to print the image (see print_label_command)

*/
function get_label()
{
	// Update the serial number with the modbus contens
	var serial = mapping[7]*65536 + mapping[8]*1;
	var hexa = parseInt(serial, 10).toString(16).toUpperCase();
	
	// Update the serial number of the label
	qrcode.value = "123456_" + hexa;
	title.value = "NOVBOX " + hexa;
	
	var printOrder = (mapping[21] == 1) ? true : false;
	
	// Return label layers
	return JSON.stringify({
		 width: 626,
		 height: 744,
		 dpi: 300,
		 launchPrint: printOrder, // boolean to actually launch the print, or not
		 printer: "command",
		 layers:[background, qrcode, title, battery_voltage, battery_capacity] // order is important, start with backround layer and then upper ones
	});
}
