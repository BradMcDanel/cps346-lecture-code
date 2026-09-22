#!/usr/bin/env python3

import gzip
import hashlib
import shutil
import urllib.request
from pathlib import Path


BASE_URL = "https://ossci-datasets.s3.amazonaws.com/mnist/"
FILES = {
    "train-images-idx3-ubyte.gz": ("f68b3c2dcbeaaa9fbdd348bbdeb94873", 47040016),
    "train-labels-idx1-ubyte.gz": ("d53e105ee54ea40749a09fcbcd1e9432", 60008),
    "t10k-images-idx3-ubyte.gz": ("9fb629c4189551a2d022fa330f9573f3", 7840016),
    "t10k-labels-idx1-ubyte.gz": ("ec29112dd5afa0611ce80d1b7f02629c", 10008),
}


def md5(path):
    digest = hashlib.md5()
    with path.open("rb") as source:
        for block in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def download(name, checksum, data_dir):
    destination = data_dir / name
    if destination.exists() and md5(destination) == checksum:
        print(f"Found {name}")
        return destination

    temporary = data_dir / (name + ".part")
    print(f"Downloading {name}")
    request = urllib.request.Request(BASE_URL + name, headers={"User-Agent": "cps346"})
    with urllib.request.urlopen(request) as response, temporary.open("wb") as output:
        shutil.copyfileobj(response, output)

    if md5(temporary) != checksum:
        raise RuntimeError(f"Checksum failed for {name}")
    temporary.replace(destination)
    return destination


def extract(source, expected_size):
    destination = source.with_suffix("")
    if destination.exists() and destination.stat().st_size == expected_size:
        print(f"Found {destination.name}")
        return

    temporary = destination.with_suffix(destination.suffix + ".part")
    print(f"Extracting {source.name}")
    with gzip.open(source, "rb") as compressed, temporary.open("wb") as output:
        shutil.copyfileobj(compressed, output)

    if temporary.stat().st_size != expected_size:
        raise RuntimeError(f"Size check failed for {destination.name}")
    temporary.replace(destination)


def main():
    data_dir = Path(__file__).resolve().parent / "data"
    data_dir.mkdir(exist_ok=True)

    for name, (checksum, size) in FILES.items():
        extract(download(name, checksum, data_dir), size)

    print("MNIST is ready in data/")


if __name__ == "__main__":
    main()
