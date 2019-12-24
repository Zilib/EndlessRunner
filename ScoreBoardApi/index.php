
<?php
header('Content-Type: application/json');
    $HostName = 'localhost';
    $db = 'EndlessRunner';
    $username = 'root';
    $password = '';

    try {
        $dbh = new PDO('mysql:host=localhost;dbname=EndlessRunner',$username,$password);
        $stmt = $dbh->prepare('SELECT * FROM scoreboard');
        $stmt->execute();
        $results = $stmt->fetchAll(PDO::FETCH_ASSOC);
        $payload = json_encode($results[0]);

        print $payload;
    }   catch (PDOException $e) {
        echo "nie Działa";
    } 
?>