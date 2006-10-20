<?php
   require('database.php');
   require('user.php');
   session_start();
   $user_id = $_SESSION['email'];
   
   database_connect();
   $result = retrieve_user($user_id);
   $admin = $result[0]['admin'];

   if ($_SERVER['REQUEST_METHOD'] == "GET")
   {
      $email = $_REQUEST['email'];
      $result = retrieve_user($email);
      $row = $result[0];
      $first = $row['first_name'];
      $last = $row['last_name'];
      $admin_priv = $row['admin'];
   }
   database_disconnect();

   if ($admin == 1 || $user_id == $email)
   {
      if ($_SERVER['REQUEST_METHOD'] == "POST")
      {
         $record_id = $_POST['record_id'];
         $email = $_POST['email'];
         $first = $_POST['first_name'];
         $last = $_POST['last_name'];
         $password1 = $_POST['password1'];
         $password2 = $_POST['password2'];
            
         if ($password1 != "" && $password1 == $password2)
            $new_password = $password1;
         else
            $password_error = "<p>Password mismatch</p>";
            
         if ($admin)
            $admin_priv = isset($_REQUEST['admin']) ? 1 : 0;
         if (!$admin && $_REQUEST['admin'])
            $admin_error = "<p>Only an admin can change admin field</p>";
            
         database_connect();
         modify_user($record_id, $email, $first, $last, $admin_priv, $new_password);
         database_disconnect();
         header('location:manage.php');
      }
?>
      <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
      <input type="hidden" name="record_id" value="<?php echo $email; ?>" />
      <table>
         <tr>
            <td>First Name:</td>
            <td><input type="text" name="first_name" value="<?php echo $first; ?>" /></td>
         </tr>
         <tr>
            <td>Last Name:</td>
            <td><input type="text" name="last_name" value="<?php echo $last; ?>" /></td>
         </tr>
         <tr>
            <td>Email:</td>
            <td><input type="text" name="email" value="<?php echo $email; ?>" /></td>
         </tr>
         <tr>
            <td>Password:</td>
            <td><input type="password" name="password1" value="" /></td>
         </tr>
         <tr>
            <td>Re-enter Password:</td>
            <td><input type="password" name="password2" value="" /></td>
         </tr>
<?php if ($user_id != $email)
      { 
?>
         <tr>
            <td>Admin Privileges:</td>
            <td><input type="checkbox" name="admin" <?php if ($admin_priv) echo 'checked="checked"';?> /></td>
         </tr>
      </table>
<?php
      }
?>
      <input type="submit" value="Submit" />
      </form>
<?php
   }
   else
   {
      echo '<p>Access Denied</p>';
   }
?>