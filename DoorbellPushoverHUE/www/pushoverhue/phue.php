<?php
class Phue {
	function Call_Light() {
		$strReturn = '';
		$client = new \Phue\Client('[IP address 192.168.0.1]', '[username]');

		try {
			$client->sendCommand(
			new \Phue\Command\Ping
			);
			$strReturn = "Bridge is live! \n";
		} catch (\Phue\Transport\Exception\ConnectionException $e) {
			$strReturn = "There was a problem accessing the bridge \n";
		}

		$isAuthenticated = $client->sendCommand(
		new \Phue\Command\IsAuthorized
		);

		$strReturn = $isAuthenticated
		? 'You are authenticated!'
		: 'You are not authenticated!';
		$strReturn = PHP_EOL;

		// From the client
		foreach ($client->getLights() as $lightId => $light) {
			$strReturn = "Id #{$lightId} - {$light->getName()} \n";
		}

		// Get a specific light
		$light = $client->getLights()[1];

		// Retrieving light properties:
		$strReturn = 'ID ' . $light->getId() . '
		Name '. $light->getName(). '
		Type'. $light->getType(). '
		ModelId '. $light->getModelId(). '
		Version '. $light->getSoftwareVersion(). '
		Status '. $light->isOn(). '
		Alert '. $light->getAlert(). '
		Brightness '. $light->getBrightness(). '
		Hue '. $light->getHue(). '
		Saturation '. $light->getSaturation(). '
		X-axis '. $light->getXY()['x']. '
		Y-axis '. $light->getXY()['y']. '
		Effect '. $light->getEffect(). '
		Colortemp '. $light->getColorTemp(). '
		Colormode '. $light->getColorMode();
		
		$intBrightness = $intHue = $intSturation = $intStatus = $intStatus = $intColortemp = $strColorMode = $intXaxis = $intYaxis = $strEffect = 0;

		$intStatus = $light->isOn();
		$intBrightness = $light->getBrightness();
		$intHue = $light->getHue();
		$intSturation = $light->getSaturation();
		$intColortemp = $light->getColorTemp();
		$intXaxis = $light->getXY()['x'];
		$intYaxis = $light->getXY()['y'];
		$strColorMode = $light->getColorMode();


		if ($light->isOn()) {
			// Setting on/off state (true|false)
			$light->setOn(true);

			// Setting brightness (0 for no light, 255 for max brightness)
			$light->setBrightness(255);

			// Set hue (0 to 65535), pairs with saturation, changes color mode to 'hs'
			$light->setHue(530);

			// Set saturation (0 min, 255 max), pairs with hue, changes color mode to 'hs'
			$light->setSaturation(150);

			// Set color temp (153 min, 500 max), changes color mode to 'ct'
			$light->setColorTemp(170);

			// Setting alert (select|lselect)
			$light->setAlert('select');

			// Set effect (none|colorloop)
			$light->setEffect('colorloop');

			sleep(2);

			// Set effect (none|colorloop)
			$light->setEffect('none');

			// Setting on/off state (true|false)
			$light->setOn(true);

			// Setting alert (select|lselect)
			//$light->setAlert('select');

			// Set color temp (153 min, 500 max), changes color mode to 'ct'
			$light->setColorTemp($intColortemp);

			// Setting brightness (0 for no light, 255 for max brightness)
			$light->setBrightness($intBrightness);

			// Set hue (0 to 65535), pairs with saturation, changes color mode to 'hs'
			$light->setHue($intHue);

			// Set saturation (0 min, 255 max), pairs with hue, changes color mode to 'hs'
			$light->setSaturation($intSturation);

			// Set xy, CIE 1931 color space (from 0.0 to 1.0 for both x and y)
			// Changes color mode to 'xy'
			$light->setXY($intXaxis, $intYaxis);
		}
		return $strReturn;
	}
}
?>