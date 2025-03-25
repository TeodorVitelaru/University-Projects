// Graph.java
package com.example.demo1.service;
import java.util.*;


public class Graph {
    private Map<Long, List<Long>> adjList;
    private int numNodes;

    public Graph(int numNodes) {
        this.numNodes = numNodes;
        this.adjList = new HashMap<>();
    }

    public void addEdge(Long node1, Long node2) {
        adjList.computeIfAbsent(node1, k -> new ArrayList<>()).add(node2);
        adjList.computeIfAbsent(node2, k -> new ArrayList<>()).add(node1);
    }

    public int countConnectedComponents() {
        Set<Long> visited = new HashSet<>();
        int count = 0;

        for (long i = 1; i <= numNodes; i++) {
            if (!visited.contains(i)) {
                count++;
                dfs(i, visited);
            }
        }
        return count;
    }

    private void dfs(Long node, Set<Long> visited) {
        visited.add(node);
        for (Long neighbor : adjList.getOrDefault(node, Collections.emptyList())) {
            if (!visited.contains(neighbor)) {
                dfs(neighbor, visited);
            }
        }
    }

    public List<Long> findLongestPathComponent() {
        List<Long> longestPath = new ArrayList<>();
        Set<Long> visited = new HashSet<>();

        for (Long node : adjList.keySet()) {
            if (!visited.contains(node)) {
                List<Long> currentPath = new ArrayList<>();
                dfsLongestPath(node, new ArrayList<>(), visited, currentPath, longestPath);
            }
        }
        return longestPath;
    }


    private void dfsLongestPath(Long current, List<Long> path, Set<Long> visited, List<Long> currentPath, List<Long> longestPath) {
        visited.add(current);
        path.add(current);

        if (path.size() > longestPath.size()) {
            longestPath.clear();
            longestPath.addAll(path);
        }

        for (Long neighbor : adjList.getOrDefault(current, Collections.emptyList())) {
            if (!visited.contains(neighbor)) {
                dfsLongestPath(neighbor, path, visited, currentPath, longestPath);
            }
        }

        path.remove(current);
        visited.remove(current);
    }
}