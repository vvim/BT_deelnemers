-- phpMyAdmin SQL Dump
-- version 4.2.12deb2+deb8u2
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Aug 15, 2016 at 03:27 PM
-- Server version: 5.5.50-0+deb8u1
-- PHP Version: 5.6.24-0+deb8u1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `buurtijd_test`
--

-- --------------------------------------------------------

--
-- Table structure for table `t_deelnemers`
--

CREATE TABLE IF NOT EXISTS `t_deelnemers` (
`id` int(10) unsigned NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `naam` varchar(50) NOT NULL,
  `familienaam` varchar(30) DEFAULT NULL,
  `straat` varchar(50) DEFAULT NULL,
  `huisnr` varchar(10) DEFAULT NULL,
  `busnr` varchar(10) DEFAULT NULL,
  `postcode` varchar(8) DEFAULT NULL,
  `plaats` varchar(30) DEFAULT NULL,
  `telefoon` varchar(15) DEFAULT NULL,
  `gsm` varchar(15) DEFAULT NULL,
  `email1` varchar(30) DEFAULT NULL,
  `email2` varchar(30) DEFAULT NULL,
  `geslacht` tinyint(3) unsigned DEFAULT NULL,
  `geboortedatum` date DEFAULT NULL,
  `afkomst` varchar(30) DEFAULT NULL,
  `statuut` tinyint(3) unsigned DEFAULT NULL,
  `bt_leren_kennen` tinyint(3) unsigned DEFAULT NULL,
  `niveau_nl` tinyint(3) unsigned DEFAULT NULL,
  `hr_goedgekeurd` tinyint(1) NOT NULL,
  `contact_delen` tinyint(1) NOT NULL,
  `fam_verzekering` tinyint(1) NOT NULL,
  `brand_verzekering` tinyint(1) NOT NULL,
  `fotomateriaal_gebruiken` tinyint(1) NOT NULL,
  `wanneer_best_contacteren` varchar(200) DEFAULT NULL,
  `contactvoorkeur` tinyint(3) unsigned DEFAULT NULL,
  `contactpersoon_voornaam` varchar(30) DEFAULT NULL,
  `contactpersoon_familienaam` varchar(30) DEFAULT NULL,
  `domein` varchar(20) DEFAULT NULL COMMENT 'How to save an Array of Ints in MySQL? Dirty solution',
  `vrijwilligers_verzekering` tinyint(1) NOT NULL,
  `doelgroep` varchar(20) DEFAULT NULL COMMENT 'How to save an Array of Ints in MySQL? Dirty solution',
  `lid` tinyint(1) NOT NULL,
  `ingeschreven_door` tinyint(3) unsigned DEFAULT NULL,
  `inschrijf_datum` date DEFAULT NULL,
  `laatste_contact` date DEFAULT NULL,
  `was_lid_is_nu_gestopt` tinyint(1) NOT NULL,
  `stop_datum` date DEFAULT NULL,
  `stop_reden` varchar(250) DEFAULT NULL,
  `soort_deelnemer` tinyint(3) unsigned NOT NULL,
  `varia` varchar(1000) DEFAULT NULL
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_deelnemer_contact_voorkeur`
--

CREATE TABLE IF NOT EXISTS `t_deelnemer_contact_voorkeur` (
  `id` tinyint(3) unsigned NOT NULL,
  `voorkeur` varchar(22) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `t_deelnemer_contact_voorkeur`
--

INSERT INTO `t_deelnemer_contact_voorkeur` (`id`, `voorkeur`) VALUES
(0, 'maakt niet uit'),
(1, 'per sms of telefoon'),
(2, 'sms'),
(3, 'bellen op vaste lijn'),
(4, 'bellen op gsm'),
(5, 'bel vaste lijn of gsm'),
(6, 'email'),
(7, 'spreek mij aan'),
(8, 'per sms of email');

-- --------------------------------------------------------

--
-- Table structure for table `t_deelnemer_doelgroep`
--

CREATE TABLE IF NOT EXISTS `t_deelnemer_doelgroep` (
  `id` tinyint(3) unsigned NOT NULL,
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
  `id` tinyint(3) unsigned NOT NULL,
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
  `id` tinyint(3) unsigned NOT NULL,
  `geslacht` varchar(5) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `t_deelnemer_geslacht`
--

INSERT INTO `t_deelnemer_geslacht` (`id`, `geslacht`) VALUES
(0, 'man'),
(1, 'vrouw');

-- --------------------------------------------------------

--
-- Table structure for table `t_deelnemer_hoe_leren_kennen`
--

CREATE TABLE IF NOT EXISTS `t_deelnemer_hoe_leren_kennen` (
  `id` tinyint(3) unsigned NOT NULL,
  `info` varchar(23) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `t_deelnemer_hoe_leren_kennen`
--

INSERT INTO `t_deelnemer_hoe_leren_kennen` (`id`, `info`) VALUES
(0, 'mond-aan-mond reclame'),
(1, 'via folder of promo'),
(2, 'via organisatie'),
(3, 'tijdens een infomoment'),
(4, 'ander');

-- --------------------------------------------------------

--
-- Table structure for table `t_deelnemer_ingeschreven_door`
--

CREATE TABLE IF NOT EXISTS `t_deelnemer_ingeschreven_door` (
  `id` tinyint(3) unsigned NOT NULL,
  `medewerker` varchar(6) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `t_deelnemer_ingeschreven_door`
--

INSERT INTO `t_deelnemer_ingeschreven_door` (`id`, `medewerker`) VALUES
(0, 'Wim'),
(1, 'Erica'),
(2, 'Bram'),
(3, 'Sultan');

-- --------------------------------------------------------

--
-- Table structure for table `t_deelnemer_niv_nederlands`
--

CREATE TABLE IF NOT EXISTS `t_deelnemer_niv_nederlands` (
  `id` tinyint(3) unsigned NOT NULL,
  `niveau` varchar(31) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `t_deelnemer_niv_nederlands`
--

INSERT INTO `t_deelnemer_niv_nederlands` (`id`, `niveau`) VALUES
(0, 'geen kennis van het Nederlands'),
(1, 'een paar woorden'),
(2, 'duidelijke zinnen'),
(3, 'vloeiend of moedertaal');

-- --------------------------------------------------------

--
-- Table structure for table `t_deelnemer_soort`
--

CREATE TABLE IF NOT EXISTS `t_deelnemer_soort` (
  `id` int(11) NOT NULL,
  `soort` varchar(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `t_deelnemer_soort`
--

INSERT INTO `t_deelnemer_soort` (`id`, `soort`) VALUES
(0, 'individu'),
(1, 'organisatie');

-- --------------------------------------------------------

--
-- Table structure for table `t_deelnemer_soort_deelnemer`
--

CREATE TABLE IF NOT EXISTS `t_deelnemer_soort_deelnemer` (
  `id` tinyint(3) unsigned NOT NULL,
  `soort` varchar(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `t_deelnemer_soort_deelnemer`
--

INSERT INTO `t_deelnemer_soort_deelnemer` (`id`, `soort`) VALUES
(0, 'individu'),
(1, 'organisatie');

-- --------------------------------------------------------

--
-- Table structure for table `t_deelnemer_statuut`
--

CREATE TABLE IF NOT EXISTS `t_deelnemer_statuut` (
  `id` tinyint(3) unsigned NOT NULL,
  `statuut` varchar(80) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `t_deelnemer_statuut`
--

INSERT INTO `t_deelnemer_statuut` (`id`, `statuut`) VALUES
(0, 'ziekenkas'),
(1, 'invaliditeitsuitkering'),
(2, 'werkzoekend met uitkering'),
(3, 'werkzoekend met PWA'),
(4, 'sociale tewerkstelling (art 60, SINE, …)'),
(5, 'OCMW Leefloon'),
(6, 'pensioen'),
(7, 'brugpensioen'),
(8, 'werk in loondienst'),
(9, 'werk als zelfstandige'),
(10, 'leerling of student'),
(11, 'werkzoekend zonder uitkering'),
(12, 'geen inkomen en niet werkzoekend');

--
-- Indexes for dumped tables
--

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
-- Indexes for table `t_deelnemer_soort_deelnemer`
--
ALTER TABLE `t_deelnemer_soort_deelnemer`
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
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
