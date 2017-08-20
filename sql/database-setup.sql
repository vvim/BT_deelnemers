-- phpMyAdmin SQL Dump
-- version 4.6.6
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: Apr 11, 2017 at 05:34 PM
-- Server version: 5.5.54-MariaDB
-- PHP Version: 5.6.30

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `buurtijd_test`
--

-- --------------------------------------------------------

--
-- Table structure for table `transactie`
--

CREATE TABLE `transactie` (
  `id` int(10) UNSIGNED NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `ontvanger` varchar(255) NOT NULL,
  `gever` varchar(255) NOT NULL,
  `buren` double UNSIGNED NOT NULL,
  `taak` varchar(255) NOT NULL,
  `datum` date NOT NULL,
  `opmerking` varchar(255) DEFAULT NULL,
  `ingegeven_door` varchar(255) DEFAULT NULL,
  `categorie` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='overzicht van de Buurtijd_boekhouding';

-- --------------------------------------------------------

--
-- Table structure for table `t_deelnemers`
--

CREATE TABLE IF NOT EXISTS `t_deelnemers` (
  `id` int(10) UNSIGNED NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `naam` varchar(50) NOT NULL,
  `familienaam` varchar(30) DEFAULT NULL,
  `straat` varchar(50) DEFAULT NULL,
  `huisnr` varchar(10) DEFAULT NULL,
  `busnr` varchar(10) DEFAULT NULL,
  `postcode` varchar(8) DEFAULT NULL,
  `plaats` varchar(30) DEFAULT NULL,
  `telefoon` varchar(15) DEFAULT NULL,
  `gsm` varchar(15) DEFAULT NULL,
  `email1` varchar(100) DEFAULT NULL,
  `email2` varchar(100) DEFAULT NULL,
  `geslacht` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `geboortedatum` date NOT NULL DEFAULT '1800-01-01',
  `afkomst` varchar(30) DEFAULT NULL,
  `statuut` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `bt_leren_kennen` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `niveau_nl` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `hr_goedgekeurd` tinyint(1) NOT NULL,
  `contact_delen` tinyint(1) NOT NULL,
  `fam_verzekering` tinyint(1) NOT NULL,
  `brand_verzekering` tinyint(1) NOT NULL,
  `fotomateriaal_gebruiken` tinyint(1) NOT NULL,
  `wanneer_best_contacteren` varchar(200) DEFAULT NULL,
  `contactvoorkeur` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `contactpersoon_voornaam` varchar(30) DEFAULT NULL,
  `contactpersoon_familienaam` varchar(30) DEFAULT NULL,
  `domein` varchar(20) DEFAULT NULL COMMENT 'How to save an Array of Ints in MySQL? Dirty solution',
  `vrijwilligers_verzekering` tinyint(1) NOT NULL,
  `doelgroep` varchar(20) DEFAULT NULL COMMENT 'How to save an Array of Ints in MySQL? Dirty solution',
  `lid` tinyint(1) NOT NULL,
  `ingeschreven_door` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `inschrijf_datum` date NOT NULL DEFAULT '1800-01-01',
  `laatste_contact` date NOT NULL DEFAULT '1800-01-01',
  `was_lid_is_nu_gestopt` tinyint(1) NOT NULL,
  `stop_datum` date NOT NULL DEFAULT '1800-01-01',
  `stop_reden` varchar(250) DEFAULT NULL,
  `soort_deelnemer` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `varia` varchar(1000) DEFAULT NULL,
  `lat` decimal(10,6) DEFAULT NULL COMMENT 'latitude up to 6 decimals (accuracy to 1 meter)',
  `lng` decimal(10,6) DEFAULT NULL COMMENT 'longitude up to 6 decimals (accuracy to 1 meter)'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

--
-- Table structure for table `t_deelnemer_contact_voorkeur`
--

CREATE TABLE IF NOT EXISTS `t_deelnemer_contact_voorkeur` (
  `id` tinyint(3) UNSIGNED NOT NULL,
  `voorkeur` varchar(22) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `t_deelnemer_contact_voorkeur`
--

INSERT INTO `t_deelnemer_contact_voorkeur` (`id`, `voorkeur`) VALUES
(0, '--nog niet ingevuld'),
(1, 'maakt niet uit'),
(2, 'per sms of telefoon'),
(3, 'sms'),
(4, 'bellen op vaste lijn'),
(5, 'bellen op gsm'),
(6, 'bel vaste lijn of gsm'),
(7, 'email'),
(8, 'spreek mij aan'),
(9, 'per sms of email');

-- --------------------------------------------------------

--
-- Table structure for table `t_deelnemer_doelgroep`
--

CREATE TABLE IF NOT EXISTS `t_deelnemer_doelgroep` (
  `id` tinyint(3) UNSIGNED NOT NULL,
  `doelgroep` varchar(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `t_deelnemer_doelgroep`
--

INSERT INTO `t_deelnemer_doelgroep` (`id`, `doelgroep`) VALUES
(0, 'iedereen'),
(1, 'armoede of kansarmoede'),
(2, 'handicap of beperking'),
(3, 'vrouwen'),
(4, 'mensen van allochtone afkomst'),
(5, 'senioren'),
(6, 'werkzoekenden'),
(7, 'jeugd en kinderen'),
(8, 'buurtbewoners'),
(9, 'psychiatrisch profiel');

-- --------------------------------------------------------

--
-- Table structure for table `t_deelnemer_domein`
--

CREATE TABLE IF NOT EXISTS `t_deelnemer_domein` (
  `id` tinyint(3) UNSIGNED NOT NULL,
  `domein` varchar(40) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `t_deelnemer_domein`
--

INSERT INTO `t_deelnemer_domein` (`id`, `domein`) VALUES
(0, 'tewerkstelling'),
(1, 'huisvesting'),
(2, 'onderwijs'),
(3, 'jeugdwerk'),
(4, 'kinderopvang'),
(5, 'vormingswerk voor volwassenen'),
(6, 'begeleiding voor specifieke doelgroepen'),
(7, 'vrije tijd'),
(8, 'buurtcomité'),
(9, 'voeding'),
(10, 'cultuur'),
(11, 'arbeidszorg'),
(12, 'milieu'),
(13, 'diensten');

-- --------------------------------------------------------

--
-- Table structure for table `t_deelnemer_geslacht`
--

CREATE TABLE IF NOT EXISTS `t_deelnemer_geslacht` (
  `id` tinyint(3) UNSIGNED NOT NULL,
  `geslacht` varchar(5) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `t_deelnemer_geslacht`
--

INSERT INTO `t_deelnemer_geslacht` (`id`, `geslacht`) VALUES
(0, '-geen'),
(1, 'man'),
(2, 'vrouw');

-- --------------------------------------------------------

--
-- Table structure for table `t_deelnemer_hoe_leren_kennen`
--

CREATE TABLE IF NOT EXISTS `t_deelnemer_hoe_leren_kennen` (
  `id` tinyint(3) UNSIGNED NOT NULL,
  `info` varchar(23) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `t_deelnemer_hoe_leren_kennen`
--

INSERT INTO `t_deelnemer_hoe_leren_kennen` (`id`, `info`) VALUES
(0, '--nog niet ingevuld'),
(1, 'mond-aan-mond reclame'),
(2, 'via folder of promo'),
(3, 'via organisatie'),
(4, 'tijdens een infomoment'),
(5, 'ander');

-- --------------------------------------------------------

--
-- Table structure for table `t_deelnemer_ingeschreven_door`
--

CREATE TABLE IF NOT EXISTS `t_deelnemer_ingeschreven_door` (
  `id` tinyint(3) UNSIGNED NOT NULL,
  `medewerker` varchar(6) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `t_deelnemer_ingeschreven_door`
--

INSERT INTO `t_deelnemer_ingeschreven_door` (`id`, `medewerker`) VALUES
(0, '-geen'),
(1, 'Wim'),
(2, 'Erica'),
(3, 'Bram'),
(4, 'Sultan'),
(5, 'Lin');

-- --------------------------------------------------------

--
-- Table structure for table `t_deelnemer_niv_nederlands`
--

CREATE TABLE IF NOT EXISTS `t_deelnemer_niv_nederlands` (
  `id` tinyint(3) UNSIGNED NOT NULL,
  `niveau` varchar(31) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `t_deelnemer_niv_nederlands`
--

INSERT INTO `t_deelnemer_niv_nederlands` (`id`, `niveau`) VALUES
(0, '--nog niet ingevuld'),
(1, 'geen kennis van het Nederlands'),
(2, 'een paar woorden'),
(3, 'duidelijke zinnen'),
(4, 'vloeiend of moedertaal');

-- --------------------------------------------------------

--
-- Table structure for table `t_deelnemer_notas`
--

CREATE TABLE IF NOT EXISTS `t_deelnemer_notas` (
  `id` int(10) UNSIGNED NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `nota` text NOT NULL COMMENT 'see http://stackoverflow.com/a/13182846/',
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------
  `deelnemer_id` int(10) UNSIGNED NOT NULL

--
-- Table structure for table `t_deelnemer_soort`
--

CREATE TABLE IF NOT EXISTS `t_deelnemer_soort` (
  `id` tinyint(3) UNSIGNED NOT NULL,
  `soort` varchar(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `t_deelnemer_soort`
--

INSERT INTO `t_deelnemer_soort` (`id`, `soort`) VALUES
(0, '--onbekend'),
(1, 'individu'),
(2, 'organisatie');

-- --------------------------------------------------------

--
-- Table structure for table `t_deelnemer_statuut`
--

CREATE TABLE `t_deelnemer_statuut` (
  `id` tinyint(3) UNSIGNED NOT NULL,
  `statuut` varchar(80) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `t_deelnemer_statuut`
--

INSERT INTO `t_deelnemer_statuut` (`id`, `statuut`) VALUES
(0, '--nog niet ingevuld'),
(1, 'ziekenkas'),
(2, 'invaliditeitsuitkering'),
(3, 'werkzoekend met uitkering'),
(4, 'werkzoekend met PWA'),
(5, 'sociale tewerkstelling (art 60, SINE, …)'),
(6, 'OCMW Leefloon'),
(7, 'pensioen'),
(8, 'brugpensioen'),
(9, 'werk in loondienst'),
(10, 'werk als zelfstandige'),
(11, 'leerling of student'),
(12, 'werkzoekend zonder uitkering'),
(13, 'geen inkomen en niet werkzoekend');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `transactie`
--
ALTER TABLE `transactie`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `id` (`id`);

--
-- Indexes for table `t_deelnemers`
--
ALTER TABLE `t_deelnemers`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `t_deelnemer_contact_voorkeur`
--
ALTER TABLE `t_deelnemer_contact_voorkeur`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `t_deelnemer_doelgroep`
--
ALTER TABLE `t_deelnemer_doelgroep`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `t_deelnemer_domein`
--
ALTER TABLE `t_deelnemer_domein`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `t_deelnemer_geslacht`
--
ALTER TABLE `t_deelnemer_geslacht`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `t_deelnemer_hoe_leren_kennen`
--
ALTER TABLE `t_deelnemer_hoe_leren_kennen`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `t_deelnemer_ingeschreven_door`
--
ALTER TABLE `t_deelnemer_ingeschreven_door`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `t_deelnemer_niv_nederlands`
--
ALTER TABLE `t_deelnemer_niv_nederlands`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `t_deelnemer_notas`
--
ALTER TABLE `t_deelnemer_notas`
  ADD PRIMARY KEY (`id`),
  ADD KEY `fk_deelnemer_id` (`deelnemer_id`);

--
-- Indexes for table `t_deelnemer_soort`
--
ALTER TABLE `t_deelnemer_soort`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `t_deelnemer_statuut`
--
ALTER TABLE `t_deelnemer_statuut`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `t_deelnemers`
--
ALTER TABLE `t_deelnemers`
MODIFY `id` int(10) unsigned NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=1;
--
-- AUTO_INCREMENT for table `t_deelnemer_notas`
--
ALTER TABLE `t_deelnemer_notas`
MODIFY `id` int(10) unsigned NOT NULL AUTO_INCREMENT;
--
-- Constraints for dumped tables
--

--
-- Constraints for table `t_deelnemer_notas`
--
ALTER TABLE `t_deelnemer_notas`
  ADD CONSTRAINT `fk_deelnemer_id` FOREIGN KEY (`deelnemer_id`) REFERENCES `t_deelnemers` (`id`);

--
-- Table structure for table `t_vraag_aanbod`
--

CREATE TABLE `t_vraag_aanbod` (
  `id` int(10) UNSIGNED NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `deelnemer` int(11) NOT NULL,
  `vraag` tinyint(1) NOT NULL COMMENT 'TRUE voor vraag, FALSE voor aanbod',
  `categorie` int(11) NOT NULL,
  `inhoud` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `t_vraag_aanbod`
--
ALTER TABLE `t_vraag_aanbod`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `id` (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `t_vraag_aanbod`
--
ALTER TABLE `t_vraag_aanbod`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=0;COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
