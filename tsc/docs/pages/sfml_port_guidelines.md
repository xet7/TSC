Guidelines for the SFML port
============================

When working on the SFML port, please adhere to the following
guidelines:

1. Do not add new features. Only port existing features from the old
   `devel` branch to the new `feature-sfml-port` branch.
2. Rewrite code only where necessary; try to stick as close to the old
   code as possible.
3. Before you start working, take some time and understand how the new
   scene/actor-based system works. Start with the `main()` function in
   `main.cpp` and follow the  flow of code until you get to the
   cLevel_Player::Update(). That should give you a thorough
   intrudocution to the system.
4. Follow the general coding guidelines as outlined in the
   `conventions` documentation page.
