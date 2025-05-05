= Test 1 =
$> ./tce19w0jtka0kzpn6kvurv7t
$> ./tce19w0jtka0kzpn6kvurv7t
$> diff -U 3 user_output_test1 test1.output | cat -e
Diff OK :D

= Test 2 =
$> ./iitv595gxfmbrkwrk0chk3fp 22
$> ./iitv595gxfmbrkwrk0chk3fp 22
$> diff -U 3 user_output_test2 test2.output | cat -e
Diff OK :D

= Test 3 =
$> ./tester ./zkl51q3cp4gt7gpvce46hbll generated3.test 15478715 | sort
$> ./tester ./zkl51q3cp4gt7gpvce46hbll generated3.test 15478715 | sort
$> diff -U 3 user_output_test3 test3.output | cat -e
Diff OK :D

= Test 4 =
$> ./tester ./wnkirdl90rpugcmo9vdk037s generated4.test 5577058 | sort
$> ./tester ./wnkirdl90rpugcmo9vdk037s generated4.test 5577058 | sort
$> diff -U 3 user_output_test4 test4.output | cat -e
Diff OK :D

= Test 5 =
$> ./tester ./nmx76b2ly6mjolxoxeyhodrv generated5.test 25696836 | sort
$> ./tester ./nmx76b2ly6mjolxoxeyhodrv generated5.test 25696836 | sort
$> diff -U 3 user_output_test5 test5.output | cat -e
Diff OK :D

= Test 6 =
$> ./tester ./4nt6nbefycc3nhp5sstsc41et generated6.test 21021253 | sort
$> ./tester ./4nt6nbefycc3nhp5sstsc41et generated6.test 21021253 | sort
$> diff -U 3 user_output_test6 test6.output | cat -e
--- user_output_test6	2025-04-17 13:26:26.474692164 +0000$
+++ test6.output 2025-04-17 13:26:27.986699407 +0000$
@@ -1,4 +1,6 @@$
+received 1 : client 1 : $
+received 1 : client 1 : $
 received 1 : server : client 1 just arrived$
received 2 : client 0 : $
received 2 : client 0 : $
Diff KO :(
Grade: 0
= Final grade: 0 =
