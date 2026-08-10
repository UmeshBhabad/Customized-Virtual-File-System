<h1 align="center">💾 Customized Virtual File System (CVFS)</h1>

<p align="center">
A simulated, in-memory Unix-style virtual file system with an interactive shell, built in C using an inode-based architecture inspired by classic Unix internals.
</p>

<hr>

<h2>📌 Project Overview</h2>

<p>
This project simulates the core internal data structures and operations of a Unix file system — Boot Block, Super Block, Inode List, File Table, and per-process User File Descriptor Table (UFDT) — entirely in memory, with an interactive command-line shell for creating, reading, writing, seeking within, and deleting files.
</p>

<p>
It's modeled directly on the classic Unix file system design (as described in Maurice Bach's <i>"The Design of the UNIX Operating System"</i>), reimplementing the layered structure that connects a process's open file descriptors to the underlying inode metadata and data buffers.
</p>

<ul>
<li>Understanding how a real file system tracks open files, metadata, and permissions</li>
<li>Understanding the layered relationship between UFDT → File Table → Inode</li>
<li>Practicing low-level C: manual memory management, linked lists, structs, and pointer-based data modeling</li>
<li>Building a command-line shell/interpreter from scratch</li>
</ul>

<hr>

<h2>⚙️ Features</h2>

<ul>
<li>Interactive shell supporting core file operations: create, read, write, seek, delete, list, and stat</li>
<li>Inode-based architecture with a linked list of fixed inodes (DILB — disk inode list block)</li>
<li>Per-process User File Descriptor Table (UFDT) mapping file descriptors to open file state</li>
<li>Independent read and write offset tracking per open file, with POSIX-<code>lseek</code>-style repositioning (START / CURRENT / END)</li>
<li>Permission system (Read / Write / Read+Write) enforced on file operations</li>
<li>Per-file statistics view (inode number, size, permission, reference count)</li>
<li>Built-in <code>man</code> pages and <code>help</code> command for shell usage</li>
<li>Defensive error handling with named error codes for invalid input, missing files, permission denial, and capacity limits</li>
</ul>

<h3>Supported Shell Commands</h3>

<ul>
<li><code>creat &lt;filename&gt; &lt;permission&gt;</code> — create a new file (permission: 1=Read, 2=Write, 3=Read+Write)</li>
<li><code>write &lt;fd&gt;</code> — write data into an open file</li>
<li><code>read &lt;fd&gt; &lt;size&gt;</code> — read a given number of bytes from a file</li>
<li><code>seek &lt;fd&gt; &lt;offset&gt; &lt;whence&gt;</code> — reposition a file's read/write offset (whence: 0=START, 1=CURRENT, 2=END)</li>
<li><code>stat &lt;filename&gt;</code> — display statistical information about a file</li>
<li><code>unlink &lt;filename&gt;</code> — delete a file and free its inode</li>
<li><code>ls</code> — list all existing files with inode number and size</li>
<li><code>man &lt;command&gt;</code> — display the manual page for a command</li>
<li><code>help</code> — display all available commands</li>
<li><code>clear</code> — clear the terminal</li>
<li><code>exit</code> — terminate the shell and release resources</li>
</ul>

<hr>

<h2>🛠️ Tech Stack</h2>

<ul>
<li><b>Language:</b> C</li>
</ul>

<h3>Concepts Used</h3>

<ul>
<li>Operating Systems — file system internals (inodes, file tables, UFDT)</li>
<li>Linked Lists (dynamic inode allocation)</li>
<li>Structs &amp; Pointers</li>
<li>Manual Dynamic Memory Management (<code>malloc</code>/<code>free</code>)</li>
<li>Command-Line Shell / Interpreter Design</li>
<li>Defensive Error Handling with Named Error Codes</li>
</ul>

<hr>

<h2>📂 Project Structure</h2>

<pre>
customized-virtual-file-system/
│
├── CVFS.cpp               # Complete implementation (shell + file system logic)
├── README.md              # Project documentation
</pre>

<hr>

<h2>🚀 Installation</h2>

<h3>1️⃣ Clone the Repository</h3>
<pre>
git clone https://github.com/UmeshBhabad/Customized-Virtual-File-System.git
cd Customized-Virtual-File-System
</pre>

<h3>2️⃣ Compile the Program</h3>
<pre>
g++ CVFS.cpp -o cvfs
</pre>

<hr>

<h2>▶️ Usage</h2>

<pre>
./cvfs
</pre>

<hr>

<h2>🖥️ How It Works</h2>

<ol>
<li>On startup, the system initializes the Super Block, allocates a fixed pool of inodes (DILB), and sets up the process's UAREA/UFDT</li>
<li>The shell enters an infinite read-eval loop, parsing each command the user types</li>
<li><code>creat</code> finds a free inode and a free UFDT slot, links them together, and returns a file descriptor</li>
<li><code>write</code>/<code>read</code> operate on an open file descriptor, tracking independent read/write offsets and enforcing permission checks before touching the data buffer</li>
<li><code>seek</code> repositions a file's offset without reading or writing any data, supporting START/CURRENT/END-relative positioning</li>
<li><code>stat</code> displays a file's current metadata without modifying anything</li>
<li><code>unlink</code> frees a file's data buffer, resets its inode to unused, and returns the inode to the free pool</li>
<li><code>exit</code> terminates the shell and releases all resources</li>
</ol>

<hr>

<h2>🧪 Example Interaction</h2>

<pre>
----- CVFS started succesfully -----

CVFS : > creat Demo.txt 3
Total number of Inodes remaining : 5
File gets succesfully created with FD 3

CVFS : > write 3
Enter the data that you want to write :
HelloWorldSeekTest
18 bytes gets succesfully written

CVFS : > seek 3 0 0
Seek successful. New offset : 0

CVFS : > read 3 19
Read operation is succesful
Data from file is : HelloWorldSeekTest

CVFS : > seek 3 5 0
Seek successful. New offset : 5

CVFS : > read 3 5
Read operation is succesful
Data from file is : World

CVFS : > seek 3 -3 2
Seek successful. New offset : 15

CVFS : > read 3 3
Read operation is succesful
Data from file is : est

CVFS : > stat Demo.txt
-----------------------------------------------
Inode Number      : 1
File Name         : Demo.txt
File Size (max)   : 50
Actual File Size  : 18
Permission        : 3
Reference Count   : 1
-----------------------------------------------

CVFS : > unlink Demo.txt
File gets succesfully deleted

CVFS : > exit
Thank you for using CVFS
</pre>

<p><i>Note: the seek examples above demonstrate all three whence modes — START (absolute position), and END with a negative offset (position relative to end-of-file).</i></p>

<hr>

<h2>🧠 Design Highlights</h2>

<ul>
<li><b>Layered File Access Model</b> — mirrors real Unix design: a file descriptor maps to a per-process File Table entry, which points to a shared Inode holding the actual metadata and data — the same indirection real operating systems use to support multiple open handles to one file</li>
<li><b>Independent Read/Write Offsets</b> — each open file tracks its own read and write cursor separately, rather than a single shared position, unlike real Unix. <code>seek</code> repositions both cursors together to the same computed position, keeping behavior predictable given this existing design choice</li>
<li><b>Fixed-Pool Inode Allocation</b> — inodes are pre-allocated as a linked list at startup (DILB) and recycled on delete, rather than allocated/freed individually per file</li>
<li><b>Defensive Error Handling</b> — every operation validates its inputs and returns named, specific error codes (invalid parameter, no inodes, file not found, permission denied, insufficient space) rather than generic failure</li>
</ul>

<h3>Known Limitations</h3>

<ul>
<li>Fully in-memory — no persistence; all files are lost when the program exits (this is a simulation of file system <i>internals</i>, not a real on-disk file system)</li>
<li>Fixed capacity: 5 inodes, 50-byte max file size, 20 max open files — sized for demonstrating the architecture, not real-world scale</li>
<li>Read and write offsets are repositioned together by <code>seek</code> rather than as independently seekable cursors, since the project already models them as two separate values by design</li>
</ul>

<hr>

<h2>🔮 Future Enhancements</h2>

<ul>
<li>Persist file system state to disk so data survives across runs</li>
<li>Support directories and hierarchical paths, not just a flat namespace</li>
<li>Dynamic inode pool sizing instead of a fixed compile-time limit</li>
<li>Symbolic/hard link support, using the existing <code>ReferenceCount</code> field</li>
<li>True bitmask-based permission checking to correctly support combined flags (e.g. Read+Write+Execute together)</li>
</ul>

<hr>

<h2>👨‍💻 Author</h2>

<p>
<b>Umesh Shivaji Bhabad</b><br>
📫 umeshbhabad9@gmail.com
</p>

<hr>

<h2>⭐ Support</h2>

<p>If you find this project useful, consider giving it a ⭐ on GitHub!</p>