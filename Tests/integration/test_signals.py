#!/usr/bin/env python3
"""Interactive signal checks for the minishell prompt and heredoc modes."""

import os
import pty
import re
import select
import signal
import sys
import time

PROMPT = b"minishell$ "
BINARY = os.path.abspath("../Assignments/minishell")


def read_until(fd, marker, timeout=4.0):
    end = time.monotonic() + timeout
    data = bytearray()
    while marker not in data:
        remaining = end - time.monotonic()
        if remaining <= 0:
            raise AssertionError(f"timed out waiting for {marker!r}: {data!r}")
        ready, _, _ = select.select([fd], [], [], remaining)
        if not ready:
            continue
        chunk = os.read(fd, 4096)
        if not chunk:
            raise AssertionError(f"minishell closed its terminal: {data!r}")
        data.extend(chunk)
    return bytes(data)


def send_command(fd, command):
    os.write(fd, command.encode() + b"\n")
    return read_until(fd, PROMPT)


def require_status(fd, expected):
    output = send_command(fd, "echo $?")
    output = re.sub(rb"\x1b\[\?[0-9;]*[hl]", b"", output)
    pattern = rb"(?:^|[\r\n])" + str(expected).encode() + rb"(?:\r\n|\n)"
    if re.search(pattern, output) is None:
        raise AssertionError(f"expected $?={expected}, got {output!r}")


def main():
    pid, fd = pty.fork()
    if pid == 0:
        os.execv(BINARY, [BINARY])
    try:
        read_until(fd, PROMPT)
        os.write(fd, b"\x03")
        read_until(fd, PROMPT)
        require_status(fd, 130)

        os.write(fd, b"\x1c")
        time.sleep(0.1)
        require_status(fd, 0)

        os.write(fd, b"/bin/sleep 5\n")
        time.sleep(0.2)
        os.write(fd, b"\x03")
        read_until(fd, PROMPT)
        require_status(fd, 130)

        os.write(fd, b"/bin/sleep 5\n")
        time.sleep(0.2)
        os.write(fd, b"\x1c")
        read_until(fd, PROMPT)
        require_status(fd, 131)

        os.write(fd, b"cat << MINISHELL_EOF\n")
        read_until(fd, b"> ")
        os.write(fd, b"\x03")
        read_until(fd, PROMPT)
        require_status(fd, 130)

        os.write(fd, b"\x04")
        _, status = os.waitpid(pid, 0)
        if not os.WIFEXITED(status) or os.WEXITSTATUS(status) != 0:
            raise AssertionError(f"Ctrl-D exit status was {status}")
        print("PASS  prompt/execution/heredoc signals and Ctrl-D")
    except Exception:
        try:
            os.kill(pid, signal.SIGKILL)
        except ProcessLookupError:
            pass
        os.waitpid(pid, 0)
        raise
    finally:
        os.close(fd)


if __name__ == "__main__":
    try:
        main()
    except Exception as error:
        print(f"FAIL  {error}", file=sys.stderr)
        sys.exit(1)
