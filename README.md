# Future Untracked Tasks Arranger

A simple ImGui-based app to do adjusted plannings, holding self-made filters to easily adjust tasks to the current moment.

## Instructions of Use

### FUTA Preferences

Preferences covers the filtering capabilities of FUTA. Which are divided in four segments:
<ul>
	<li>Filtering save options: save or restore default options to have them available right when starting the program.</li>
	<li>Ordering options: by name, starting date or by importance. Whenever a task is added or edited, the filtering doesn't occur automatically (it was this
	or to have ImGui detect a change before the user was done editing, which would re-order tasks mid-editing. Trust me, this is better); to trigger it, either select
	the ordering option again, or press "Rearrange again".</li>
	<li>"Only x" filtering options: only shows tasks that have deadlines, which are progressible, which have been started, or which are not completed.</li>
	<li>"Exclude x" filtering options: hides tasks that are performed mindlessly, with minimal focus or with maximum focus.</li>
</ul>

### FUTA Tasks

Section where the tasks are shown. It has an extra creation button, and completed tasks deletion button for convenience's sake.
For each task, there are the following variables:
<ul>
	<li>Name: name of the task, with a 75 character limit. They can be repeated, since tasks have time-based UID as identifiers.</li>
	<li>Started checkbox: whether a task has been started.</li>
	<li>Completed checkbox: whether a task has been completed.</li>
	<li>Delete task: button to delete the task (has a confirmation in case of missclick).</li>
	<li>State: defines the current state of the task.</li>
	<li>Effort type: defines the mind state in which the task should be performed.</li>
	<li>Importance: a value of priority for filtering.</li>
	<li>Starting date: date to start the task.</li>
	<li>Ending date: date to end the task (my brain is so big).</li>
	<li>Deadline checkbox: whether the task has a deadline or is more of a chill 'TODO'.</li>
	<li>Task description: if there's anything to write about the task, here you have 250 free characters to do so.</li>
	<li>Subtasks: a simplified task menu to split a task into smaller, more affordable tasks.</li>
</ul>

That would be about it, it's not that bad to learn, aye? :3

## Name disclaimer

If you are wondering about the name, I had done another application called ASS (Audio Semi-Automatic Selector, something for karaoke selection). So in my head, it
would be hilarious to send a message to my friends about how I "decided to get my hands on FUTA after I had enjoyed using my ASS", or something along the line. Regardless
of how embarassing the joke became after, like, five minutes, I have committed myself to it for too far to change all the code and project name. No regrets.

