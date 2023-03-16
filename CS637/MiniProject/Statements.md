# LTL

## Theory

### Valid logical operators

Logical Operator | Description

1 | true

0 | false

! | not

& | and

| | or

^ | xor

-> | implies

---

### Valid temporal modal operators

Modal Operators | Description

X | next

F | eventually

G | globally

U | strong until

R | weak release

M | strong release

W | weak until

---

## Home

- Turn on the TV and change to my most viewed channel.

```py
    F (turn_on_tv & (X ((save_most_viewed_channel_in_variableX) & (X (change_channel_to_variableX)))))
```

- Robot1 prepare a the usual coffee and Robot2 bring it to me.

```py
    F (save_usual_coffee_in_variableX & F (((!coffee_machine_busy) -> (robot1_make_coffee_variableX) & X F (robot2_pickup_coffee_from_machine & X (save_user_location_in_variableY & F(robot2_reach_location_variableY))))))
```

- Draw up the living room's window blinds at sunrise.

```py
    G ((check_sunrise) -> (X (F (draw_window_blinds))))
```

---

## School

- Erase the second whiteboard.

```py
    F((save_second_whiteboard_location_in_variableX) & F (reach_location_variableX & X F (clean_whiteboard)))
```

- Take the attendance by using 3 different angled cameras and crossmatching them.

```py
    save_picture_camera1_in_variableP1 & save_picture_camera2_in_variableP2 & save_picture_camera3_in_variableP3 & clear_variableS & F (append_identified_people_of_variableP1_in_variableS & append_identified_people_of_variableP2_in_variableS & append_identified_people_of_variableP3_in_variableS & X F ((save_popped_variableS_in_variableP & X (mark_attendance_of_person_variableP)) U (variableS_empty)))
```

- Dust off the duster.

```py
    F(duster_pickup & X (hit_duster_on_hard_surface W (duster_clean)))
```

---

## Hospital

- Take out 3 pills of the second medicine box at 10pm.

```py
    (G(save_time_in_variableT) & check_if_variableT_is_10pm) -> (X (F (unscrew_second_medicine_box & F (take_out_three_pills))))
```

- Read through the given prescription and order the medicines mentioned.

```py
    F (save_prescription_items_in_variableL & X F (order_popped_variableL U variableL_empty))
```

- Dispose off the used up syringes.

```py
    F (pick_used_syringes_box & X (save_disposalSystem_location_in_variableX & X F (G(avoid_collisions) & reach_location_variableX & X F (release_used_syringes_box))))
```

---

## Factory

- Arrange "n" finish products in a packaging carton box.

```py
    F(robot1_ready_box &  ((save_detected_empty_space_in_variablePos & X F (robot2_put_product_at_variablePos)) U (box_contains_n_products | box_full)))
```

- Arrange "k" packaging boxes and wrap them for export.

```py
    robot1_allot_grouping_area & X G ((save_found_empty_space_in_variablePos & put_box_at_variablePos) U (grouping_area_contains_k_boxes | grouping_area_full))
```

- Bring a box of ID "i" from the storage room and empty that in a sequence on the conveyer belt.

```py
    F (robot1_bring_box_ID_i & X F ((pick_item_out_box & put_item_on_conveyer) U box_empty))
```

---

## Garage

- Open up the garage shutter.

```py
    F (!check_garage_shutter_open -> push_garage_button)
```

- Monitor the charge of the car battery, keeping it ready to move by 8am everyday.

<!-- assuming that 12 hours would be enough to charge the car whatever be its percent -->
```py
    (G (save_time_in_variableT) & check_variableT_greater_than_8pm & car_present) -> X (save_battery_status_in_variableBatt & X (save_calculated_time_to_charge_from_variableBatt_to_full_in_variableNeed & X (start_charging U time_variableNeed_has_passed | time_is_equal_8am)))
```

- Find the instrument/component of id "i" from the toolbox and place it on the table.

```py
    save_location_table_in_variableTable & (save_location_instrument_IDi_in_variablePos) & X F (robot1_pick_tool_at_variablePos & X F (robot1_reach_variableTable & X F (robot1_release_tool)))
```

---

## Garden

- Increase hydration by 10% of the sapling at (i,j) position on the grid that the garden represents.

```py
    save_location_posx_i_posy_j_in_variablePos & (save_hydration_amt_at_variablePos_in_variableHy) & X F (G(avoid_collision) & reach_pos & X F (water_till_hydration_is_1.1_times_variableHy & X (update_hydration_amt_at_variablePos)))
```

- Empty the watering can and place it inside the garden shed.

```py
    F (pour_watering_can_on_nearest_sink U watering_can_empty) -> X F (reach_garden_shed & F (place_watering_can_inside))
```

- Monitor the blossoming of plant at (i,j) and harvest fully blossomed flowers.

```py
    save_posx_i_posy_j_in_variablePos & G (check_mature_flower_at_variablePos -> F (reach_variablePos & X F (pluck_mature_flowers)))
```

---

## Shop

- Check the state of the currency notes given by the customer, reject the non-acceptable ones.

```py
    G ((collect_note & put_collected_note_on_stack) R (!top_note_check_valid -> return_note_to_last_customer))
```

- Stack coins in the inventory according to their denomination.

```py
    G (new_coin_entered_inventory R (pass_last_coin_through_sorter -> 1))
```

- Pick out every item of id "i" from a heap of items.

```py
    G (F (pick_item -> X F (item_id_i_put_heap_1 ^ item_id_not_i_put_heap_2)) U heap_empty)
```

---

## CS Lab

- Roll down the projector screen when the projector is turned on, and vice versa.

```py
    G((check_projector_on -> roll_down_projector) | (!check_projector_on -> !roll_down_projector))
```

- Turn down the intensity of other lights while the projector is ON.

```py
    G ((check_projector_on -> X F (turn_lights_intensity_to_20percent)) | !check_projector_on)
```

- Monitor and log the health of each computer at 6pm everyday.

```py
    G (checK_time_equal_6pm -> X (log_computers_health) | !check_time_equal_6pm)
```

```py
G(patient_vomiting -> F(tablet_locate & X F(tablet_reach & X tablet_pick & X (patient_locate & X F (patient_reach & X (tablet_give))))))

F(stethoscope_reach & X stethoscope_bring & X F (reach_patient & ) )

G(time_is_1000 -> X (planters_pick & X (planters_place_under_sun U time_is_1200)))

F(((Robot3_check_temperature) & (X(temperature_less))) -> 
    ( ((!(temperature_less)) | ((notify_robot4) & (X(robot4_switch_on_cooling)))) -> 
        ((notify_robot4) & (X(Robot4_switch_off_cooling)))))

GF(((Robot3_check_temperature) ->
    ((temperature_high & X (switch_on_cooling)) | (temperature_low & X (switch_off_cooling)))
  ))

(lab_time_up) -> G ((F ((reach_unvisited_computer) & ((unvisited_computer_state_on) -> X (switch_off)))) U (all_computers_powered_off))

F ((container_reach) & X G (((take_sugar_out_10grams) & X (put_sugar_on_container)) U (container_contains_2kg)))

F (((counter_set_0) & (X((increase_counter_by_1) & (X(decrease_coins_counter_by_1))))) U (no_coins_left))

G (patient_arrives -> X (scan_injury & X (rate_injury & X (reorder_priority_list))))

F ((reach_toolbox) & (X(F((pickup_toolbox) & (locate_user & F (reach_user & X drop_toolbox))))))

G (((pick_tool) & X F (put_tool_in_toolbox)) U (no_tool_left))

F ((robot1_climb_up_ladder) & X (!(robot1_lookup_item -> (robot1_locate_item & X F (robot1_pick_item))) & X F (robot1_climb_down_ladder)))

G ((approach_unchecked_computer & X check_computer_status) U ((all_computers_checked | status_fault) -> (X(report_staff))))

G ((log_pollution_sensor -> (check_abnormality -> (abnormality_detected & X report_log | abnormality_not_detected))))
```
