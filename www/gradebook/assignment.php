<?php
function assignment_create($class_key, $title, $category_key, $max_points, $due_date, $rank)
{
   // Query string should contain properly formatted SQL
   $query = "INSERT INTO assignment 
            VALUES(null,'{$class_key}','{$title}','{$category_key}','{$max_points}','{$due_date}','{$rank}')";
   $result = mysql_query($query);
   if (!$result)
      return false;
   return true;
}

function assignment_edit($assignment_key, $title, $category_key, $max_points, $due_date, $rank)
{
   if (assignment_exists($assignment_key))
   {
      // Query string should contain properly formatted SQL, will want to update     
      // only changed information
      $updates = "title='{$title}',category_key='{$category_key}',max_points='{$max_points}',due_date='{$due_date}',rank='{$rank}'";
      $query = "UPDATE assignment SET {$updates} WHERE assignment_key='{$assignment_key}'";
      $result = mysql_query($query);
      if (!$result)
         return false;
      return true;
   }
   else
      return false;
}

// We never want to delete things in most of these classes.
// If there were a large number of entries in the database it could wreak havoc to delete a class. So instead we will set a flag to hide a class. If a teacher really wants a class to disappear we might need a separate front end that is dedicated to hazardous operations
function assignment_delete($assignment_key)
{
   if (assignment_exists($assignment_key))
   {
      // Query string should contain properly formatted SQL
      $query = "UPDATE assignment SET is_active='0' WHERE assignment_key={$assignment_key}";
      $result = mysql_query($query);
      if (!$result)
         return false;
      return true;
   }
   else
      return false;
}

function assignment_get($assignment_key)
{
   // Query string should contain properly formatted SQL
   $query = "SELECT * FROM assignment WHERE assignment_key='{$assignment_key}'";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
      $results[] = $row;
   return $results;
}

function assignment_get_all()
{
   // Query string should contain properly formatted SQL
   $query = "SELECT * FROM assignment";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
      $results[] = $row;

   return $results;
}

function assignment_exists($assignment_key)
{
   $result = assignment_get($assignment_key);
   if ($result)
      return true;
   else
      return false;
}
?>