<?php

function make_tiles($width) {
	$im = imagecreatetruecolor($width  * 40 - 1, 79);
	$red = imagecolorallocate($im, 255, 0, 0);
	$black = imagecolorallocate($im, 0, 0, 0);
	
	$top = imagecreatefrompng("Tiles/tileBrown_02.png");
	$bottom = imagecreatefrompng("Tiles/tileBrown_27.png");
	
	imagecolortransparent($im, $black);
	
	for($i = 0; $i < $width; $i++) {
		imagecopyresized($im, $bottom, $i * 40, 40, 0, 0, 39, 39, 64, 64);
		imagecopyresized($im, $top, $i * 40, 0, 0, 0, 39, 39, 64, 64);
	}
	
	
	imagepng($im, "{$width}x2_mono.png");
	
	imagedestroy($top);
	imagedestroy($bottom);
	
	imagedestroy($im);
}

function make_platform($width, $height) {
	$im = imagecreatetruecolor($width  * 40 - 1, $height * 40 - 1);
	$red = imagecolorallocate($im, 255, 0, 0);
	$black = imagecolorallocate($im, 0, 0, 0);
	
	$left = imagecreatefrompng("Tiles/tileBrown_01.png");
	$right = imagecreatefrompng("Tiles/tileBrown_03.png");

	$one = imagecreatefrompng("Tiles/tileBrown_04.png");
	
	$top = imagecreatefrompng("Tiles/tileBrown_02.png");
	$bottom = imagecreatefrompng("Tiles/tileBrown_27.png");
	
	imagecolortransparent($im, $black);
	
	for($i = 0; $i < $width; $i++) {
		for($j = 1; $j < $height; $j++) {
			imagecopyresized($im, $bottom, $i * 40, $j * 40, 0, 0, 39, 39, 64, 64);
		}
		
		$imtocopy = $top;
		if ($width == 1) {
			$imtocopy = $one;
		} else {
			if ($i == 0) {
				$imtocopy = $left;
			}
			
			if ($i == $width - 1) {
				$imtocopy = $right;
			}
			
		}
		
		imagecopyresized($im, $imtocopy, $i * 40, 0, 0, 0, 39, 39, 64, 64);
	}
	
	
	imagepng($im, "{$width}x{$height}.png");

	imagedestroy($left);
	imagedestroy($right);
	
	imagedestroy($one);
	
	imagedestroy($top);
	imagedestroy($bottom);
	
	imagedestroy($im);
}



function make_one_height_platform($width) {
	$im = imagecreatetruecolor($width  * 40 - 1, 40);
	$red = imagecolorallocate($im, 255, 0, 0);
	$black = imagecolorallocate($im, 0, 0, 0);
	
	$left = imagecreatefrompng("Tiles/tileBrown_05.png");
	$right = imagecreatefrompng("Tiles/tileBrown_07.png");

	$one = imagecreatefrompng("Tiles/tileBrown_08.png");
	
	$top = imagecreatefrompng("Tiles/tileBrown_06.png");
	
	imagecolortransparent($im, $black);
	
	for($i = 0; $i < $width; $i++) {
		$imtocopy = $top;
		if ($width == 1) {
			$imtocopy = $one;
		} else {
			if ($i == 0) {
				$imtocopy = $left;
			}
			
			if ($i == $width - 1) {
				$imtocopy = $right;
			}
			
		}
		
		imagecopyresized($im, $imtocopy, $i * 40, 0, 0, 0, 39, 39, 64, 64);
	}
	
	
	imagepng($im, "{$width}x1.png");

	imagedestroy($left);
	imagedestroy($right);
	
	imagedestroy($one);
	
	imagedestroy($top);
	
	imagedestroy($im);
}


$arSizes = array(1, 3, 5, 10, 20);
foreach($arSizes as $size) {
	make_tiles($size);
}

$arSizes = array(1, 3, 5);
foreach($arSizes as $size) {
	make_platform($size, 2);
}

$arWidths = array(1, 2);
$arHeights = array(2, 4, 6);
foreach($arWidths as $width) {
	foreach($arHeights as $height) {
		make_platform($width, $height);
	}
}

$arWidths = array(2, 3, 5);
foreach($arWidths as $size) {
	make_one_height_platform($size, 2);
}

