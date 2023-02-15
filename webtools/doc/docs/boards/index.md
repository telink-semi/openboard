# 泰凌开源系统

!!! note "介绍"
    泰凌开源系统主要有开源模块板，开源代码，开源工具组成。

## Commands

* `mkdocs new [dir-name]` - Create a new project.
* `mkdocs serve` - Start the live-reloading docs server.
* `mkdocs build` - Build the documentation site.
* `mkdocs -h` - Print help message and exit.

## Project layout

    mkdocs.yml    # The configuration file.
    docs/
        index.md  # The documentation homepage.
        ...       # Other markdown pages, images and other files.

!!! note "这是 note 类型的提示框"
提示：更多精彩内容记得关注我啊

!!! success "这是 success 类型的提示框"
成功！

!!! failure "这是 failure 类型的提示框"
失败！

!!! bug "这是 bug 类型的提示框"
发现一个 bug，请尽快修复！


??? question "How to add plugins to the Docker image?"

    Material for MkDocs only bundles selected plugins in order to keep the size
    of the official image small. If the plugin you want to use is not included, 
    create a new `Dockerfile` and extend the official Docker image:

    ``` Dockerfile
    FROM squidfunk/mkdocs-material
    RUN pip install ...
    ```

    Next, you can build the image with the following command:

    ```
    docker build -t squidfunk/mkdocs-material .
    ```

    The new image can be used exactly like the official image.

!!! info ":material-apple: Apple Silicon (M1) and :fontawesome-brands-raspberry-pi: Raspberry Pi"

    The official Docker image is only available for `linux/amd64`. We recommend
    the [third-party image] by @afritzler if you want to run Material for MkDocs
    via Docker on `arm64` or `armv7`, as it is automatically built on every
    release:

    ```
    docker pull ghcr.io/afritzler/mkdocs-material
    ```

  [third-party image]: https://github.com/afritzler/mkdocs-material


!!! Abstract "example"

    === "Unordered List"

        ``` markdown
        * Sed sagittis eleifend rutrum
        * Donec vitae suscipit est
        * Nulla tempor lobortis orci
        ```

    === "Ordered List"

        ``` markdown
        1. Sed sagittis eleifend rutrum
        2. Donec vitae suscipit est
        3. Nulla tempor lobortis orci
        ```

=== "C"

    ``` c
    #include <stdio.h>

    int main(void) {
      printf("Hello world!\n");
      return 0;
    }
    ```

=== "C++"

    ``` c++
    #include <iostream>

    int main(void) {
      std::cout << "Hello world!" << std::endl;
      return 0;
    }
    ```
## 串口命令格式
!!! note "set advertising interval"
    串口发送命令：01 ff 02 00 50 00

    命令解析：0x50=80，interval=80 *0.625ms

    串口返回数据：FF 03 01 07 00

!!! note "set advertising data"
    串口发送命令：02 ff 06 00 05 09 01 02 03 04

    命令解析：0x05：数据长度+1  01 02 03 04：广播数据 

    串口返回数据：FF 03 02 07 00

!!! note "enable/disable advertising"
    串口发送命令：0a ff 01 00 01或0a ff 01 00 00

    串口返回数据：FF 03 0A 07 00

!!! note "set device name"
    串口发送命令：13 ff 0a 00 01 02 03 04 05 06 07 08 09 0a

    命令解析：0x0a：device name长度， 0x01 02 03 04 05 06 07 08 09 0a：device name

    串口返回数据：FF 03 13 07 00

!!! note "terminate connection"
    串口发送命令：17 ff 00 00

    串口返回数据：FF 03 17 07 00

!!! note "restart module"
    串口发送命令：18 ff 00 00

    串口返回数据：FF 03 18 07 00

!!! note "get module current work state"
    串口发送命令：16 ff 00 00

    串口返回数据：FF 04 16 07 00 01（未连接） FF 04 16 07 00 08（连接态）

!!! note "send notify data"
    串口发送命令：1c ff 07 00 11 00 01 02 03 04 05

    命令解析：0x07：长度+2  0x01 02 03 04 05：数据

    串口返回数据：FF 03 1C 07 00
