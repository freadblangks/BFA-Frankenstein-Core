-- Panicking Worker
SET @ENTRY := 48331;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,4,0,100,1,0,0,0,0,11,36099,0,0,0,0,0,2,0,0,0,0,0,0,0,'Cast Throw Hammer on Aggro'),
(@ENTRY,0,1,0,2,0,100,0,0,30,15000,17000,11,30615,0,0,0,0,0,2,0,0,0,0,0,0,0,'Cast Fear at 30% HP');