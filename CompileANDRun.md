# HOW TO COMPILE #

To compile the application, enter the package root directory and run the script
`build.sh`.
All the necessary steps should be automatically performed and a binary executable should be placed inside the `bin/` subdirectory. In order to view all the available compilation options, run `build.sh -h`.

# HOW TO RUN #

In order to launch VisualNetkit, run the script `bin/visualnetkit.sh`. This automatically sets the environment variable `VISUAL_NETKIT_PLUGINS` to properly locate plugins provided with the package.