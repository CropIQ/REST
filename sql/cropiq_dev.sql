SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;


CREATE TABLE `animalgroups` (
  `id` int(11) NOT NULL,
  `farmId` int(11) NOT NULL,
  `name` varchar(120) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `animalmedicaments` (
  `id` int(11) NOT NULL,
  `animalId` int(11) NOT NULL,
  `medicamentId` int(11) NOT NULL,
  `amount` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `animals` (
  `id` int(11) NOT NULL,
  `animalGroupId` int(11) NOT NULL,
  `name` varchar(120) NOT NULL DEFAULT '',
  `breed` varchar(120) NOT NULL DEFAULT '',
  `birthDateTime` timestamp NOT NULL DEFAULT current_timestamp(),
  `weight` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `farms` (
  `id` int(11) NOT NULL,
  `usersCount` int(11) NOT NULL DEFAULT 0,
  `name` varchar(120) NOT NULL DEFAULT ''
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `inventory` (
  `id` int(11) NOT NULL,
  `farmId` int(11) NOT NULL,
  `name` varchar(120) NOT NULL DEFAULT ''
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `items` (
  `id` int(11) NOT NULL,
  `inventoryId` int(11) NOT NULL,
  `orderId` int(11) DEFAULT NULL,
  `name` varchar(120) NOT NULL DEFAULT ''
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `linkedfarms` (
  `id` int(11) NOT NULL,
  `userId` int(11) NOT NULL,
  `farmId` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `medicaments` (
  `id` int(11) NOT NULL,
  `name` varchar(120) NOT NULL,
  `description` text NOT NULL,
  `measure` enum('mg','g','ml','capsule','tablet') NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `orders` (
  `id` int(11) NOT NULL,
  `userId` int(11) NOT NULL,
  `orderDate` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `reminders` (
  `id` int(11) NOT NULL,
  `farmId` int(11) NOT NULL,
  `name` varchar(120) NOT NULL,
  `dueDate` timestamp NOT NULL DEFAULT current_timestamp(),
  `assignedWorkerId` int(11) NOT NULL,
  `linkedAnimalId` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `reports` (
  `id` int(11) NOT NULL,
  `userId` int(11) NOT NULL,
  `reportType` enum('') NOT NULL,
  `name` varchar(120) NOT NULL DEFAULT '',
  `startDate` timestamp NOT NULL DEFAULT current_timestamp(),
  `endDate` timestamp NOT NULL DEFAULT current_timestamp(),
  `generatedDate` timestamp NOT NULL DEFAULT current_timestamp(),
  `contents` longtext CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL DEFAULT '' CHECK (json_valid(`contents`))
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `tasks` (
  `id` int(11) NOT NULL,
  `farmId` int(11) NOT NULL,
  `name` varchar(120) NOT NULL DEFAULT '',
  `assignedWorkerId` int(11) NOT NULL,
  `status` enum('planned','ongoing','ended') NOT NULL,
  `dueDate` timestamp NOT NULL DEFAULT current_timestamp(),
  `priority` enum('low','medium','hight') NOT NULL,
  `description` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `users` (
  `id` int(11) NOT NULL,
  `role` enum('worker') NOT NULL,
  `name` varchar(100) NOT NULL DEFAULT '',
  `email` varchar(100) NOT NULL,
  `password` varchar(60) NOT NULL,
  `contactInfo` text NOT NULL DEFAULT ''
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `workers` (
  `id` int(11) NOT NULL,
  `userId` int(11) NOT NULL,
  `role` enum('farmer','worker') NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;


ALTER TABLE `animalgroups`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `animalmedicaments`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `animals`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `farms`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `inventory`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `items`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `linkedfarms`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `medicaments`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `orders`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `reminders`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `reports`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `tasks`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `users`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `workers`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `userId` (`userId`);


ALTER TABLE `animalgroups`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `animalmedicaments`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `animals`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `farms`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `inventory`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `items`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `linkedfarms`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `medicaments`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `orders`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `reminders`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `reports`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `tasks`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `users`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `workers`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
