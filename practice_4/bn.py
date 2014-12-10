#!/usr/bin/env python3

if __name__ == "__main__":
    with open("bayesian_network", "rb") as f:
        lines = f.read().decode("ISO-8859-15").replace("\r\n", "\n").split("\n")
    vertexNames = lines[0].split()
    i = 1
    while (i < len(lines)) and (len(lines[i]) > 0):
        vp = lines[i].replace(":", " : ").split()  # [vertex, ':', probability]
        assert((len(vp) == 3) and (vp[1] == ':'))
        # TODO
        i += 1

    while i < len(lines):
        i += 1
        first = True
        while (i < len(lines)) and (lines[i] != "end"):
            if first:
                first = False
                vv = lines[i].replace(":", " : ").split()  # [vertex, ':', vertex, vertex, ...]
                assert((len(vv) >= 2) and (vv[1] == ':'))
                for j in range(2, len(vv)):
                    print("Edge (%d, %d)" % (int(vv[0]), int(vv[j])))
                    # TODO: add edge
            else:
                cp = lines[i].replace(":", " : ").split()  # [(not?) vertex, (not?) vertex, ..., ':', probability]
                assert((len(cp) >= 3) and (cp[-2] == ':'))
                # TODO
            i += 1
