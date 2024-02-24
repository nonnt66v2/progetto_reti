-- MySQL dump 10.13  Distrib 8.0.36, for Linux (x86_64)
--
-- Host: 127.0.0.1    Database: universita
-- ------------------------------------------------------
-- Server version	8.0.36-0ubuntu0.23.10.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `appello`
--

DROP TABLE IF EXISTS `appello`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `appello` (
  `data_appello` date NOT NULL,
  `id_esame` int NOT NULL,
  KEY `appello_esame_id_esame_fk` (`id_esame`),
  CONSTRAINT `appello_esame_id_esame_fk` FOREIGN KEY (`id_esame`) REFERENCES `esame` (`id_esame`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `appello`
--

LOCK TABLES `appello` WRITE;
/*!40000 ALTER TABLE `appello` DISABLE KEYS */;
INSERT INTO `appello` VALUES ('2024-02-21',12410),('2024-03-21',12410),('2024-01-21',12410),('2024-02-22',12410),('2024-02-22',12410),('2024-05-05',12410),('2024-06-06',12413),('2024-09-06',12411),('2024-02-02',12411),('2024-09-11',12411),('2024-09-11',12411),('2024-11-08',12413);
/*!40000 ALTER TABLE `appello` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `corso_di_studio`
--

DROP TABLE IF EXISTS `corso_di_studio`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `corso_di_studio` (
  `id_corso` int NOT NULL AUTO_INCREMENT,
  `nome_corso` varchar(100) NOT NULL,
  PRIMARY KEY (`id_corso`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `corso_di_studio`
--

LOCK TABLES `corso_di_studio` WRITE;
/*!40000 ALTER TABLE `corso_di_studio` DISABLE KEYS */;
INSERT INTO `corso_di_studio` VALUES (1,'informatica'),(2,'scienze nautiche'),(3,'economia'),(4,'giurisprudenza');
/*!40000 ALTER TABLE `corso_di_studio` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `esame`
--

DROP TABLE IF EXISTS `esame`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `esame` (
  `id_esame` int NOT NULL AUTO_INCREMENT,
  `anno_corso_esame` int DEFAULT NULL,
  `nome_esame` varchar(1000) DEFAULT NULL,
  `cfu_esame` int NOT NULL,
  `id_corso_di_studio` int NOT NULL,
  PRIMARY KEY (`id_esame`),
  KEY `esame_corso_di_studio_id_corso_fk` (`id_corso_di_studio`),
  CONSTRAINT `esame_corso_di_studio_id_corso_fk` FOREIGN KEY (`id_corso_di_studio`) REFERENCES `corso_di_studio` (`id_corso`),
  CONSTRAINT `check_anno_corso_esame` CHECK ((`anno_corso_esame` between 1 and 3))
) ENGINE=InnoDB AUTO_INCREMENT=12414 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `esame`
--

LOCK TABLES `esame` WRITE;
/*!40000 ALTER TABLE `esame` DISABLE KEYS */;
INSERT INTO `esame` VALUES (12410,1,'prog1',12,1),(12411,2,'ASD',12,1),(12412,2,'diritto privato',12,4),(12413,3,'tech-web',6,1);
/*!40000 ALTER TABLE `esame` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `prenota`
--

DROP TABLE IF EXISTS `prenota`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `prenota` (
  `mat_studente` varchar(11) NOT NULL,
  `id_esame` int DEFAULT NULL,
  `data_prenotazone` date DEFAULT NULL,
  KEY `prenota_studente_mat_studente_fk` (`mat_studente`),
  KEY `prenota_esame_id_esame_fk` (`id_esame`),
  CONSTRAINT `prenota_esame_id_esame_fk` FOREIGN KEY (`id_esame`) REFERENCES `esame` (`id_esame`),
  CONSTRAINT `prenota_studente_mat_studente_fk` FOREIGN KEY (`mat_studente`) REFERENCES `studente` (`mat_studente`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `prenota`
--

LOCK TABLES `prenota` WRITE;
/*!40000 ALTER TABLE `prenota` DISABLE KEYS */;
INSERT INTO `prenota` VALUES ('0124000001',12410,'2024-02-22');
/*!40000 ALTER TABLE `prenota` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `studente`
--

DROP TABLE IF EXISTS `studente`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `studente` (
  `mat_studente` varchar(11) NOT NULL,
  `password_studente` varchar(100) NOT NULL,
  `nome_studente` varchar(100) DEFAULT NULL,
  `cognome_studente` varchar(100) DEFAULT NULL,
  `anno_corso_studente` int NOT NULL,
  PRIMARY KEY (`mat_studente`),
  CONSTRAINT `check_anno_corso` CHECK ((`anno_corso_studente` between 1 and 3))
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `studente`
--

LOCK TABLES `studente` WRITE;
/*!40000 ALTER TABLE `studente` DISABLE KEYS */;
INSERT INTO `studente` VALUES ('0124000001','ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f','n1','c1',3),('0124002','f64551fcd6f07823cb87971cfb91446425da18286b3ab1ef935e0cbd7a69f68a','n2','c1',1),('0124003','f64551fcd6f07823cb87971cfb91446425da18286b3ab1ef935e0cbd7a69f68a','n3','c3',3),('20230001','ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f','Mario','Rossi',1),('20230002','fbb4a8a163ffa958b4f02bf9cabb30cfefb40de803f2c4c346a9d39b3be1b544','Luca','Bianchi',2),('20230003','703b0a3d6ad75b649a28adde7d83c6251da457549263bc7ff45ec709b0a8448b','Giulia','Verdi',3),('20230004','f8007910cb7e15ebace0b29a47404d1b0b860dc6ba2c0169dd431f667a9f3ffe','Anna','Neri',1),('202323','f64551fcd6f07823cb87971cfb91446425da18286b3ab1ef935e0cbd7a69f68a','n3','c3',3);
/*!40000 ALTER TABLE `studente` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `supera`
--

DROP TABLE IF EXISTS `supera`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `supera` (
  `mat_studente` varchar(11) DEFAULT NULL,
  `id_esame` int DEFAULT NULL,
  `voto` int DEFAULT NULL,
  KEY `supera_esame_id_esame_fk` (`id_esame`),
  KEY `supera_studente_mat_studente_fk` (`mat_studente`),
  CONSTRAINT `supera_esame_id_esame_fk` FOREIGN KEY (`id_esame`) REFERENCES `esame` (`id_esame`),
  CONSTRAINT `supera_studente_mat_studente_fk` FOREIGN KEY (`mat_studente`) REFERENCES `studente` (`mat_studente`),
  CONSTRAINT `check_name` CHECK ((`voto` between 18 and 30))
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `supera`
--

LOCK TABLES `supera` WRITE;
/*!40000 ALTER TABLE `supera` DISABLE KEYS */;
INSERT INTO `supera` VALUES ('0124003',12411,18);
/*!40000 ALTER TABLE `supera` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2024-02-24 14:56:27
