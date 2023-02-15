# Welcome to MkDocs

For full documentation visit [mkdocs.org](https://www.mkdocs.org).

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
