<?php
	$title = "CSCE 2410 - PHP Program 3";
	$section = "Assignment: Generate a Multiplication Table";
	require("../../php-template.php");
	$default_size = 12;
	$min_size = 2;
	$max_size = 12;
	$table_size = $default_size;
	
if ($_SERVER['REQUEST_METHOD'] == "POST"){
	$last_size = $_REQUEST['last_size'];
	if (isset($_POST['text_submit_button'])){
		$table_size = $_REQUEST['text_field'];
	} elseif (isset($_POST['select_submit_button'])){
		$table_size = $_REQUEST['table_select'];
	} elseif (isset($_POST['radio_submit_button'])){
		$table_size = $_REQUEST['radio_button'];
	}
	if ($table_size > $max_size or $table_size < $min_size) {
		echo "<p class=\"error\">Value out of range, must be between {$min_size} and {$max_size}</p>\n";
		$table_size = $last_size;
	} else {
		$last_size = $table_size;
	}
}

?>

<div class="form">
	<form method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
		<input type="hidden" name="last_size" value="<?php echo $last_size; ?>" />
		<div id="text_input">			
			<input name="text_field" type="text" size="3" maxlength="2" value="<?php echo $table_size; ?>" />
			<input type="submit" name="text_submit_button" value="Submit" />
		</div>
		<div id="select_input">
			<select name="table_select">
				<?php
				for ($i = $min_size; $i <= $max_size; $i++){
					echo "<option value=\"{$i}\" "; 
					if ($_SERVER['REQUEST_METHOD'] != "POST" and $i == $max_size){
						echo 'selected="selected" ';
					}
					if ($_SERVER['REQUEST_METHOD'] == "POST" and $i == $table_size){
						echo 'selected="selected" ';
					}
					echo ">{$i}</option>\n";
				}
				?>
			</select>
			<input type="submit" name="select_submit_button" value="Submit" />
		</div>
		<div id="radio_input">
			<?php
			for ($i = $min_size; $i <= $max_size; $i++){
				echo "<label for=\"Radio{$i}\">{$i}</label><input id=\"Radio{$i}\" type=\"radio\" name=\"radio_button\" value=\"{$i}\" ";
					if ($_SERVER['REQUEST_METHOD'] != "POST" and $i == $max_size){
						echo 'checked="checked" ';
					}
					if ($_SERVER['REQUEST_METHOD'] == "POST" and $i == $table_size){
						echo 'checked="checked" ';
					}
				echo "/>\n";
			}
			?>
			<input type="submit" name="radio_submit_button" value="Submit" />
		</div>
	</form>
</div>

<div>
	<table id="progtable">
<?php
echo'
		<!-- The table head -->
        <tr>
            <th>
                &nbsp;
            </th>
	';
            for ($i=1; $i <= $table_size; $i++) {
                echo "<th>{$i}</th>";
            }
echo'
		</tr>
		<!-- Generate the table -->
	';
	for ($i=1; $i <= $table_size; $i++) {
		echo"
			<tr>
				<th>{$i}</th>
			";
			for ($j=1; $j <= $table_size; $j++) {
				echo "<td>" . $i * $j. "</td>";
			}
		echo"
			</tr>
			";
	}

?>
    </table>
</div>