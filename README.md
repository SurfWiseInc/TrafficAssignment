# TrafficAssignment

 TrafficManager spawns BP_AICarPawn, child of AWheeledVehiclePawn with UChaosWheeledVehicleMovementComponent.

 SpawnRate can be set in TrafficManager with SpawnRateTime and Divergence for randomized spawn rate.
 Note: Minimum SpawnRate should not be less than 5 seconds (i.e. SpawnRateTime - Divergence > 5sek) because regardless of set  DontSpawnIfColliding Cars do collide on spawn and generate undesireable behaviour

 TrafficManager is implemented as ObjectPool to avoid spawning new agents at runtime. We can set InitialPoolSize in TrafficManager
 but if predefined size is exeeded the pool will scale accordingly. 

 Car's speed is controlled through SetThrottleInput() and SetBrakeInput(). Cars are given different ThrottleInput to have different speeds 
 to observe automatic braking in case of another car in front of them.
