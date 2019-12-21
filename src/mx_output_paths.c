#include "pathfinder.h"

static char* memory_safe_strjoin(char* str1, char* str2) {
    char* result = mx_strjoin(str1, str2);
    if (str1 != NULL) {
        free(str1);
    }
    if (str2 != NULL) {
        free(str2);
    }
    return result;
}

static char* output_path(t_pair *pair){
    char* path_str;

    path_str = memory_safe_strjoin(mx_strdup("Path: "), mx_strdup(pair->first->name));
    path_str = memory_safe_strjoin(path_str, mx_strdup(" -> "));
    path_str = memory_safe_strjoin(path_str, mx_strdup(pair->second->name));
    path_str = memory_safe_strjoin(path_str, mx_strdup("\n"));
    return path_str;
}

static void print(char *path, char *route, char *distance) {
    char *result_str;

    route = memory_safe_strjoin(mx_strdup("Route: "), route);
    route = memory_safe_strjoin(route, mx_strdup("\n"));

    distance = memory_safe_strjoin(mx_strdup("Distance: "), distance);

    result_str = memory_safe_strjoin(
                 mx_strdup("========================================\n"), path);
    result_str = memory_safe_strjoin(result_str, route);
    result_str = memory_safe_strjoin(result_str, distance);
    result_str = memory_safe_strjoin(result_str, 
                 mx_strdup("\n========================================\n"));
    mx_printstr(result_str);
    free(result_str);
}

static char *create_distance_str(t_pair *pair, struct s_path *route, 
                                 char *old_distance_str,
                                 struct s_island *current){
    char *new_distance_str;

    if (mx_strcmp(pair->first->name, route->to->name) == 0) {
        if (mx_strcmp(old_distance_str, "\0") == 0){
            new_distance_str = mx_itoa(current->shortest_path);
            free(old_distance_str);
        } else {
            new_distance_str = memory_safe_strjoin(
                               mx_itoa(route->length_of_bridge), old_distance_str);
            new_distance_str = memory_safe_strjoin(new_distance_str, 
                               mx_strdup(" = "));
            new_distance_str = memory_safe_strjoin(new_distance_str, 
                               mx_itoa(pair->second->shortest_path));
        }
    } else {
        new_distance_str = memory_safe_strjoin(mx_itoa(route->length_of_bridge), old_distance_str);
        new_distance_str = memory_safe_strjoin(mx_strdup(" + "), new_distance_str);
    }
    return new_distance_str;
}

static void fill_route_and_distance(t_pair *pair, 
                                    struct s_island *current,
                                     char *route, char *distance) {
    int i = 0;
    char *distance_str;
    char *route_str;

    if(mx_strcmp(current->name, pair->first->name) == 0) {
        print(output_path(pair), route, distance);
        return;
    }
    for (i = 0; current->best_routes[i] != NULL; i++) {
        route_str = mx_strjoin(" -> ", route);
        route_str = memory_safe_strjoin(
                    mx_strdup(current->best_routes[i]->to->name), route_str);
        distance_str = create_distance_str(pair, current->best_routes[i], 
                       mx_strdup(distance), current);
        fill_route_and_distance(pair, current->best_routes[i]->to,
                                mx_strdup(route_str), mx_strdup(distance_str));
        free(route_str);
        free(distance_str);
    }

    free(route);
    free(distance);
}

void mx_output_result(struct s_island* from, struct s_island* to){
    t_pair *pair = mx_create_pair(from, to);
    char *route = mx_strdup(to->name);
    char *distance = mx_strdup("\0");

    fill_route_and_distance(pair, to, route, distance);
    mx_free_pair(pair);
}
