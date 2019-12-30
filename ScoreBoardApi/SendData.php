<?php
if (@$_GET['PlayerName'] && $_GET['Score']) {
    echo "dsadsa";
    $HostName = 'localhost';
    $db = 'EndlessRunner';
    $username = 'root';
    $password = '';
    try {
        $dbh = new PDO('mysql:host=localhost;dbname=EndlessRunner',$username,$password);
        $stmt = $dbh->prepare('INSERT INTO scoreboard (PlayerName, Score) VALUES (?, ?);');
        $stmt->bindParam(1,$_GET['PlayerName'], PDO::PARAM_STR);
        $stmt->bindParam(2,$_GET['Score'], PDO::PARAM_INT);
        $stmt->execute();

    }   catch (PDOException $e) {
        echo "nie Działa";
    } 
}
?>