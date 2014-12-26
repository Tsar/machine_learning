#!/usr/bin/env python3

g = {}  # graph
gBack = {}  # graph
prob = {}
condP = {}

def calcProb(u):
    global prob

    if u in gBack:
        for v in gBack[u]:
            calcProb(v)

        p_ = 0
        for l in condP[u]:
            print("===")
            print(u)
            print(l)
            print(condP[u][l])
            p = condP[u][l]
            for x in l:
                if x.startswith('¬'):
                    p *= (1 - prob[int(x[-1])])
                else:
                    p *= prob[int(x)]
            p_ += p
        prob[u] = p_

if __name__ == "__main__":
    with open("bayesian_network", "rb") as f:
        lines = f.read().decode("ISO-8859-15").replace("\r\n", "\n").split("\n")
    vertexNames = lines[0].split()
    i = 1
    while (i < len(lines)) and (len(lines[i]) > 0):
        vp = lines[i].replace(":", " : ").split()  # [vertex, ':', probability]
        assert((len(vp) == 3) and (vp[1] == ':'))
        prob[int(vp[0])] = float(vp[2])
        i += 1

    while i < len(lines):
        i += 1
        first = True
        while (i < len(lines)) and (lines[i] != "end"):
            if first:
                first = False
                vv = lines[i].replace(":", " : ").split()  # [vertex, ':', vertex, vertex, ...]
                assert((len(vv) >= 2) and (vv[1] == ':'))
                u = int(vv[0])
                g[u] = []
                condP[u] = {}
                for j in range(2, len(vv)):
                    v = int(vv[j])
                    print("Edge (%d, %d)" % (u, v))
                    g[u].append(v)
                    if not v in gBack:
                        gBack[v] = []
                    gBack[v].append(u)
            else:
                cp = lines[i].replace(":", " : ").split()  # [(not?) vertex, (not?) vertex, ..., ':', probability]
                assert((len(cp) >= 3) and (cp[-2] == ':'))
                condP[u][tuple(cp[:-2])] = float(cp[-1])
            i += 1

    print(g)
    print(gBack)
    print(prob)

    for u in g:
        calcProb(u)
        print(prob)

    with open("bayesian_network.dot", "w") as f:
        f.write("digraph bn {\n")
        for u in g:
            f.write("    V%d [label=%s]\n" % (u, str(u) + "_" + str(prob[u])))
        for u in g:
            for v in g[u]:
                f.write("    V%d -> V%d\n" % (u, v))
        f.write("}\n")
