
<?
//
// Basic PHP XML Processor - Write
// We should change this to use SimpleXML its in PHP5
//
$items = Array();
function start_element($parser, $name, $attrs){
	global $items;
	if($name == "item"){
		array_push($items, $attrs);
	}
}
function end_element ($parser, $name){}
$playlist_string = file_get_contents("list.xml");
$parser = xml_parser_create();
xml_set_element_handler($parser, "start_element", "end_element");
xml_parser_set_option($parser, XML_OPTION_CASE_FOLDING, 0);
xml_parse($parser, $playlist_string) or die("Error parsing XML document.");
print "<br />";
if($_POST['action'] == "ins"){
	array_push($items, Array(
				"title" => $_POST['name'],
				"artist" => $_POST['artist'],
				"path" => $_POST['path']));
	$songs_final = $items;
}else if($_POST['action'] == "del"){
	$songs_final = Array();
	foreach($items as $item){
		if($item['title'] != $_POST['name']){
			array_push($songs_final, $item);
		}
	}
}
$write_string = "<items>\n";
foreach($songs_final as $item){
	$write_string .= "<item title=\"$item[title]\" artist=\"$item[artist]\" path=\"$item[path]\" />\n";
}
$write_string .= "</items>";
$fp = fopen("list.xml", "w+");
fwrite($fp, $write_string) or die("Error writing to file");
fclose($fp);
//print "<em>Item inserted or deleted successfully :)</em><br />";
//print "<a href=\"index.php\" title=\"return\">Return</a>";
    header('Location: '.'./?success=true');
	exit;
?>

