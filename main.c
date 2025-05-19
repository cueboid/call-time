/*
 * <a dumb program that shows the amount of time since the start of the vc in j&j>
 * Copyright (C) 2025 cuboid
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <stdint.h>

#define CALL_START_TIMESTAMP 1744316124

volatile sig_atomic_t keep_running = 1;

void set_input_echo(int enable)
{
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);

	if (enable)
		t.c_lflag |= (ICANON | ECHO);
	else
		t.c_lflag &= ~(ICANON | ECHO);

	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void handle_sigint(int sig)
{
	keep_running = 0;
	write(STDOUT_FILENO, "\n", 1);
}

int main(void)
{
	signal(SIGINT, handle_sigint);
	set_input_echo(0);
	write(STDOUT_FILENO, "\033[?25l", 6);
	while (keep_running) {
		time_t now = time(NULL);
		time_t difference = now - CALL_START_TIMESTAMP;

		long long diff = (long long)difference;

		/* ik this dosen't account for leap years */
		/* TODO: switch to gmtime or smth */
		long long total_days = diff / 86400;
		long long total_hours = diff / 3600;
		long long years = total_days / 365;
		long long days = total_days % 365;
		long long hours = (diff % 86400) / 3600;
		long long minutes = (diff % 3600) / 60;
		long long seconds = diff % 60;

		printf("\r\033[K");
		printf("since call start: %lld years, %lld days, %lld hours, "
		       "%lld minutes, %lld seconds | "
		       "total hours: %lld hours | "
		       "total days: %lld days",
		       years, days, hours, minutes, seconds,
		       total_hours, total_days);

		fflush(stdout);
		sleep(1);
	}
	write(STDOUT_FILENO, "\033[?25h", 6);
	set_input_echo(1);
	return 0;
}
