<?php 
// Disable Notices
error_reporting (E_ALL ^ E_NOTICE); 
// Also could put this in php.ini (error_reporting )
?>
<!-- http://www.odi.ch/prog/design/php/guide.php -->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd" >
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>CCS - Tests</title>
<style type="text/css">
body{
 font: normal 11px  helvetica,verdana, arial, sans-serif;
}
table {
  border-collapse: collapse;
  border: 1px solid #666666; 
  color: #363636;
  background: #f6f6f6;
  text-align:left;
  width:100%;
  }
caption {
  text-align: center;
  font: bold 16px arial, helvetica, sans-serif;
  background: transparent;
  padding:6px 4px 8px 0px;
  color: #CC00FF;
  text-transform: uppercase;
}
thead, tfoot {
background:url(bg1.png) repeat-x;
text-align:left;
height:30px;
}
thead th, tfoot th {
padding:5px;
}
table a {
color: #333333;
text-decoration:none;
}
table a:hover {
text-decoration:underline;
}
tr.odd {
background: #f1f1f1;
}
tbody th, tbody td {
padding:5px;
}
fieldset { border:0; }
label { margin-right:5px; }
</style>
</head>
<body>
<h2><a href="/" style="color:blue">Community Core Server</a> - XML Read/Write Test</h2>
<form action="process.php" method="post">
<fieldset>
<label for="name">Var 1:</label><input type="text" id="name" name="name"/><br />
<label for="artist">Var 2:</label><input type="text" id="artist" name="artist" /><br />
<label for="path">Var 3:</label><input type="text" id="path" name="path" /> <br />
<select name="action">
<option value="ins">Insert</option>
<option value="del">Delete</option>
</select>
<input type="submit" />
</fieldset>
</form>
<?php
if (isset($_GET['success'])) 
{
 $success_string .= "<div style='background:#D4FFD4; padding:10px'><a href='./'>Success...</a></div>";
 echo $success_string;
 }
function start_tag($parser, $name, $attrs){
	global $table_string;
	// Throws notice errors!
	//if (isset($attrs[title]) && isset($attrs[artist]) && isset($attrs[path])) 
    //{
     $table_string .= "<tr>
	 <td>$attrs[title]</td>
	 <td>$attrs[artist]</td>
	 <td>$attrs[path]</td>
	 </tr>";
    //}
}
function end_tag($parser, $name){}
$parser = xml_parser_create();
xml_parser_set_option($parser, XML_OPTION_CASE_FOLDING, 0);
xml_set_element_handler($parser, "start_tag", "end_tag");
$table_string = "<table>
		<tr>
		<th>Var 1</th>
		<th>Var 2</th>
		<th>Var 3</th>
		</tr>";
xml_parse($parser, file_get_contents("list.xml")) or die("Error parsing XML file");
$table_string .= "</table>";
echo $table_string;
?>
</body>
</html>