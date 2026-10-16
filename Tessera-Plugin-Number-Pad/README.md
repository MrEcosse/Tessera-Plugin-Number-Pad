# Number pad

The keys 0 to 9 on one tile, with an optional Clear and Enter, for any number a Home Assistant action takes: a TV
channel, the TV to switch to in a bar, a camera to bring up on a screen, a preset, a room. You type one action with the
number's place marked in it, and choose when it is sent.

It works with anything: the pad is not tied to a device or an integration. It sends exactly the action you type, to the
entity you choose, so it suits bespoke setups.

It is not for PIN codes. Tessera's own lock and alarm tiles open a keypad of their own that shows dots, counts wrong
codes, locks the keypad after repeated mistakes and never stores or logs the code. This tile has none of that, so leave
codes for locks and alarms to the built-in keypad.

The keys follow a telephone's layout (1 2 3, 4 5 6, 7 8 9, then Clear, 0 and Enter) where they come out large enough. On
a wide, low tile they go in rows of four or six instead, with Clear and Enter at the end. A key is never higher than
14 mm nor wider than half again its height. The tile is one cell at least and up to 4x4; twelve keys on one cell are
small, so 2x2 or taller is comfortable.

## Set up

1. In Tessera, open **Plugins**, choose **Number pad** and tick your screens. Each screen builds once with the plugin.
2. In Layout, place the **Number pad** tile from the library's Plugins group.
3. In the inspector, choose the entity the action goes to (a remote, a media player, a helper, a script), and type the
   **Action**: the action, then optionally an entity of its own, then optionally one field, with `{digit}` or `{code}`
   where the number goes (below).
4. Choose **Send**, and turn **Enter key** and **Clear key** on or off.
5. Make sure the screen may perform Home Assistant actions. Tessera turns this on for a screen it added to Home
   Assistant; for a screen added by hand, tick **Allow the device to perform Home Assistant actions** in the ESPHome
   integration's options for that device.

## The action, and where the number goes

The action text is the action, then optionally an entity of its own, then optionally one field (`name=value`). In the
field's value, two words are replaced when a key is tapped:

| Word | Is replaced by |
|---|---|
| `{digit}` | the key just tapped: `7` |
| `{code}` | the digits typed so far: `407` |

They work only in the field's value, not in the action's name or in the entity. A field's value runs to the end of the
text and may hold spaces. At most 64 characters once the number is in it.

## When it is sent

**Each digit** sends the action as soon as a digit is tapped. This is right when the device collects the digits itself,
as a TV does for a channel:

| For | Action | Enter's action |
|---|---|---|
| An IR or Android TV remote | `remote.send_command command=NUM_{digit}` | `remote.send_command command=ENTER` |

In this mode an Enter key needs an **Enter's action** of its own. `{code}` is also available, as the digits tapped so far.

**On Enter** gathers the digits, shows them above the keys, and sends the whole number when Enter is tapped. This is
right when one action takes the whole number, which is how to switch between TVs or cameras:

| For | Action |
|---|---|
| A helper that an automation reads | `input_number.set_value value={code}` |
| A text helper | `input_text.set_value value={code}` |
| A choice that takes the number | `input_select.select_option option=TV {code}` |
| A media player's source | `media_player.select_source source=Input {code}` |

A tile can send only one field, so for anything more, point the action at a helper and let a Home Assistant automation
(or a script it triggers) do the rest: it sees the number arrive and switches the TV, the camera or the channel.

Enter with nothing typed does nothing and shows red. If **Enter's action** is filled in, Enter sends that instead of the
action above; use it to send the number somewhere else, or a plain Enter command.

## On the screen

- A tap on a key lights it blue while the action goes out, or red when it could not be sent (no entity, an action that
  is not one, or the screen may not perform actions). An action that is not one shows every digit in red.
- **Most digits** (1 to 8, default 3): on Enter, further digits are refused; sending each digit, the next one starts a
  new number.
- **Forget after** (default 10 seconds, 0 for never): digits typed and not sent are forgotten after this time without a
  tap, so the next person starts afresh. Clear empties them at once, and sending the number does too.
- **Show the number** (on Enter): the digits typed so far above the keys. Turn it off to give the keys the room.
- The keys go grey while the entity is unavailable.
- Works on every board, the CYD included. Where a tile is small, the keys are small; give it more room.

## Good to know

- The action runs whatever you type, on whatever entity you name. The manifest lists the actions the tile is made for
  (remote, media player, helpers, script), but the screen does not hold it to that list: anyone who can edit the layout
  can make the pad perform any action the screen is allowed to.
- Nothing is stored: the number lives in the tile only until it is sent or forgotten, and goes nowhere but to Home
  Assistant with the action.
- Nothing leaves your home: the tile uses no internet service and no Home Assistant entity but the ones you choose.

## How it works

- `tessera-plugin.yaml`: one tile, `pad`, 1x1 to 4x4, that belongs to an entity of the domains it acts on, with the
  action, when it is sent, the two optional keys, the length, the idle time and whether to show the number.
- `components/number_pad/number_command.h`: reads an action's text into an action, a target and a field (the same code
  as the Transport controls plugin).
- `components/number_pad/number_template.h`: puts the digit and the number into the field's value.
- `components/number_pad/number_pad.cpp`: chooses the arrangement of the keys, keeps the number, finds the key under the
  finger (`tessera::tap_point`) and sends the action (`tessera::action`).
