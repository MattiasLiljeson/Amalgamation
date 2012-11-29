[33mcommit 8133cf4617eec2798657ca6c93edcadfb6590563[m
Author: Alexander Brodén <broden.alexander+github@gmail.com>
Date:   Wed Nov 28 17:57:37 2012 +0100

    BROKEN BUILD, but compiles.
    Fixed some manager compile errors in entity world.

[33mcommit df7ac81d285238bda76b28e6686ef1c1076a989f[m
Author: Alexander Brodén <broden.alexander+github@gmail.com>
Date:   Wed Nov 28 17:24:32 2012 +0100

    BROKEN BUILD!! Compiles though.
    Implemented component manager

[33mcommit 9a3b71bf95038a3fa1999953125c7ee35291bd8f[m
Merge: ca29a4b dd32436
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Wed Nov 28 15:12:40 2012 +0100

    Merge remote-tracking branch 'remotes/kod/s1u7t1_implBasicEs' into s2u7t1x1_testBasicES

[33mcommit ca29a4bd077cc747ca5b8a168a45a5be29c1d362[m
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Wed Nov 28 15:10:48 2012 +0100

    Added new test: EntitySystemTEST.
    Creation of EntitySystem works.

[33mcommit dd32436452f061681cd2c2a1c466a0d9e093cc23[m
Author: Alexander Brodén <broden.alexander+github@gmail.com>
Date:   Wed Nov 28 15:02:53 2012 +0100

    BROKEN BUILD! Compiles though. Further impl of Artemis.

[33mcommit 55a9214a678fb5366d3b32a3fae96b6cecae77a7[m
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Wed Nov 28 14:50:36 2012 +0100

    Changed SystemType class to work the exact same way as ComponentType does.

[33mcommit 9016d2bc6af5e5a8562410e9c3bb24ad126dfc3e[m
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Wed Nov 28 14:19:52 2012 +0100

    Tested ComponentType class.
    
    Mattias fixed some errors in ComponentType.

[33mcommit 4fc5f75aec53856571fc52ec04d5a1112d676827[m
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Wed Nov 28 13:27:00 2012 +0100

    Gtest is now fully functional with VS2012.
    Thanks Jarl :D

[33mcommit b2c68061c046a69ca74d864466ef1da2beb63194[m
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Wed Nov 28 13:21:26 2012 +0100

    backup...

[33mcommit cee5fd1eda9278681ff53df1856ebe22810356b0[m
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Wed Nov 28 11:35:27 2012 +0100

    Fixed an error where componentBits were used instead of systemBits.

[33mcommit d9aa2b209ec569226b1dceff89707fe336514c9a[m
Merge: 36a7618 843acdd
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Wed Nov 28 11:06:26 2012 +0100

    Merge remote-tracking branch 'remotes/kod/s1u7t1_implBasicEs' into s2u7t1x1_testBasicES

[33mcommit 843acdd461643cf4d42617e6c40cee8aeeec9a07[m
Author: Alexander Brodén <broden.alexander+github@gmail.com>
Date:   Wed Nov 28 10:54:20 2012 +0100

    Changed ignore-file and did som small fixes
    
    I don't remember what though...

[33mcommit b1c9a6d3a550cc89b219fc36b95fdec4c2c32bb6[m
Author: Alexander Brodén <broden.alexander+github@gmail.com>
Date:   Wed Nov 28 10:36:32 2012 +0100

    ES lib does now compile. Needs testing.

[33mcommit 36a761878e4352fe69c3e834436826bbae8d9e6e[m
Merge: 38b5d58 0e97f6f
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Wed Nov 28 09:37:49 2012 +0100

    Merge branch 's2x1_AddGtestProject' into s2u7t1x1_testBasicES

[33mcommit 38b5d58a8c40a060e97152d8fcb56a77d8c62596[m
Author: Mattias Liljeson <mattiasliljeson+cecilia-pc@gmail.com>
Date:   Wed Nov 28 09:29:34 2012 +0100

    BROKEN BUILD! WIP ES added to the ES-project in thsi solution

[33mcommit 0e97f6f46a8e416bb659012b14b3fe649931c204[m
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Tue Nov 27 16:40:50 2012 +0100

    Added references to all other libs. Gtest integration SHOULD be done!
    
    * Ask Johan if there are any questions!!! *

[33mcommit b8f6c074f683eebbba6d8939fd197a28bc54e83e[m
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Tue Nov 27 16:36:45 2012 +0100

    Changed outdir to be at the same place as ClientApp and ServerApp. Also changed working dir (which means we have to deal with the Gtest.vcxproj.user file.

[33mcommit b3b6f880cbb139660e8ee35331454343c10f1eab[m
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Tue Nov 27 16:32:13 2012 +0100

    Added another test case with both a header and a cpp file. It works for Gameplay project now.

[33mcommit 8080e1dbf6d8dfa1d49d9f0e9565b69ed2b030d8[m
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Tue Nov 27 16:27:52 2012 +0100

    Fixed out-dirs for google test project.

[33mcommit dd74ba2909c6bc2b7c3a1129e45e0fa6a2a9b1ae[m
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Tue Nov 27 16:20:57 2012 +0100

    Added the gtest .lib files to the repo (ignoring issue...)

[33mcommit f5b9135ec246ae6fa111127b518aff1a783d42e2[m
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Tue Nov 27 16:12:21 2012 +0100

    Moved Gtest project files into its Src folder.

[33mcommit b0d09cdf619dc8839dd6b30e0c306682da5f44f4[m
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Tue Nov 27 15:53:07 2012 +0100

    Minor changes in Gtest project filters.

[33mcommit 029ca6319a3848e1c4156c604daa4c2513f2dcd6[m
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Tue Nov 27 15:50:50 2012 +0100

    Added Gtest Runner.cpp. It works!

[33mcommit cd62fefdbb7cd4cbf0d38a433370902a056a5113[m
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Tue Nov 27 15:44:40 2012 +0100

    Added static links to the google test libs.

[33mcommit 1c36e0fda74d1f616581b169fc03dc36b070c002[m
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Tue Nov 27 15:40:14 2012 +0100

    Added paths to the Gtest project.

[33mcommit d08af0db437ae1ac1bc5d0b22c060b53dcb3c83b[m
Author: Johan Carlberg <johan.carlsberg@gmail.com>
Date:   Tue Nov 27 15:35:22 2012 +0100

    Added google test external lib and includes.

[33mcommit 3b0e805d6f0dfced629ea3b3ba5fbf24f64281a1[m
Author: Alexander Brodén <broden.alexander+github@gmail.com>
Date:   Tue Nov 27 14:14:05 2012 +0100

    Updated gitignore, added .user files

[33mcommit c9ce7ece9cab5e68059fa92084c65891832bff61[m
Author: Alexander Brodén <broden.alexander+github@gmail.com>
Date:   Tue Nov 27 14:11:55 2012 +0100

    Updated project settings

[33mcommit d6e7f1657e928f21b995e2c385df0a64f146ad2d[m
Author: Alexander Brodén <broden.alexander+github@gmail.com>
Date:   Tue Nov 27 14:10:39 2012 +0100

    Excluded .user from the ignore list

[33mcommit 786892d3d8ab9050730051a26b0764f3dccf55ce[m
Author: Alexander Brodén <broden.alexander+github@gmail.com>
Date:   Tue Nov 27 11:45:17 2012 +0100

    Added DirectX external folder to the ignore list

[33mcommit af423d623aa1c6a371db422e88babeefeea0696e[m
Author: Robin Thunström <robin.thunstrom@gmail.com>
Date:   Tue Nov 27 11:39:58 2012 +0100

    Changed working layout for projects.

[33mcommit 26e647a37d1286a4aa0d4e4764185c98a4ed7ca7[m
Author: Alexander Brodén <broden.alexander+github@gmail.com>
Date:   Tue Nov 27 11:32:33 2012 +0100

    Added project libs in the master solution

[33mcommit 43b483e6253e5760d8311b4f86dd7ae65003abb5[m
Author: Robin Thunström <robin.thunstrom@gmail.com>
Date:   Mon Nov 26 16:45:02 2012 +0100

    FORRCCEE

[33mcommit d6d26327ce626a58ae49ef425606d5fc35fb21e1[m
Author: Robin Thunström <robin.thunstrom@gmail.com>
Date:   Mon Nov 26 16:41:07 2012 +0100

    Solution working dir, propose #2

[33mcommit e56a2abfa8d7c98243322278c6cb8d194499cd1a[m
Author: Robin Thunström <robin.thunstrom@gmail.com>
Date:   Wed Nov 21 17:21:39 2012 +0100

    Made some changes to the doxygen settings file.
    
    Added so that private members are now visible.

[33mcommit f9cc3d337a39ea830f0efc277454049163fb27e2[m
Author: Robin Thunström <robin.thunstrom@gmail.com>
Date:   Tue Nov 20 14:52:51 2012 +0100

    Changed git ignore file. Changed the lines 27 and 38 into comment.

[33mcommit 73b7a96ee9c0619ff598fcc45c4e1f7e531a50fa[m
Author: Robin Thunström <robin.thunstrom@gmail.com>
Date:   Tue Nov 20 14:48:32 2012 +0100

    Added dummy files so that the whole map structure from Assets and External can be viewed.

[33mcommit a3396ba999fc4e78541253f8aeaab6c128f8e004[m
Author: Robin Thunström <robin.thunstrom@gmail.com>
Date:   Tue Nov 20 14:44:28 2012 +0100

    Changed the git ignore file to include all folders so that the map structure can be viewed more easily.
    
    Changed */bin/ to #*/bin/
    Changed [Dd]ebug*/ to #[Dd]ebug*/

[33mcommit d2a4fe19476a74184d000a11b45e7b29f49ec994[m
Author: Robin Thunström <robin.thunstrom@gmail.com>
Date:   Tue Nov 20 14:32:25 2012 +0100

    Map-structure proposal number 1.

[33mcommit 0317209cc1e207b791b3f9c3a02c7c3199d345ff[m
Author: Robin Thunström <robin.thunstrom@gmail.com>
Date:   Tue Nov 20 14:30:34 2012 +0100

    Removed dummy files.

[33mcommit dc545d19147eefce40f7529302c0f5c26517db50[m
Merge: 8bcd37c 239b1e8
Author: jarllarsson <jarl.larsson@gmail.com>
Date:   Thu Nov 15 09:38:20 2012 +0100

    Merge remote-tracking branch 'remotes/origin/s1u3t3_doxygenSetup'
    
    Resolved conflicts:
    	.gitignore

[33mcommit 239b1e8e184db9b3e45c8e28c0ad1383987065d7[m
Author: jarllarsson <jarl.larsson@gmail.com>
Date:   Thu Nov 15 09:31:35 2012 +0100

    added syntax for documenting function parameters

[33mcommit 8bcd37c297c2e09b02a17c733e0faf5af79ff03d[m
Author: Mattias Liljeson <mattiasliljeson+g503@gmail.com>
Date:   Wed Nov 14 15:30:10 2012 -0800

    Fixed gitignore
    gitignore now contains ingores for C++/visual studio instead of g++ ignores.

[33mcommit dba36d58447e066bbf7a1eeb4902672ffcab1bea[m
Author: jarllarsson <jarl.larsson@gmail.com>
Date:   Wed Nov 14 17:19:51 2012 +0100

    syntax update

[33mcommit 5684aea2d6933d1b70a1fda9aeab00635bcbfb8d[m
Author: jarllarsson <jarl.larsson@gmail.com>
Date:   Wed Nov 14 16:14:26 2012 +0100

    Added one way to handle documentation of multiple projects. Look into this further later on as we have no projects as for now.

[33mcommit 52cec01282ff085b29cb068888d4d772fd80e69e[m
Author: jarllarsson <jarl.larsson@gmail.com>
Date:   Wed Nov 14 15:51:06 2012 +0100

    Created doxyfile that generates a documentation on all files in the src-folder, recursively. The docs-folder is added to ignore.

[33mcommit 1f3ab55fbc004ec8741bd80d77992f1789cfc111[m
Author: Mattias Liljeson <mattiasliljeson@gmail.com>
Date:   Tue Nov 13 09:04:30 2012 -0800

    Initial commit
