<?php
    $redirect_url= "Location: http://" . $_SERVER['SERVER_ADDR'] . "/users/guest";
    echo $redirect_url . "\n";
    echo $_SERVER['HTTP_HOST'] . "\n";
    echo $_SERVER['HTTP_X_REAL_IP'] . "\n";
//    header($redirect_url)
?>
